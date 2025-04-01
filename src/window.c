#include "window.h"

static void unbind_all();
static void enable_vsync();
static void create_window();
static void check_resources();
static HGLRC create_temp_context();
static HGLRC create_context();

static const char* application_icon = "..\\resources\\icons\\test.ico";
static const char* application_name = "Demi";
static const char* wc_class_name = "Demi";
static _Bool key_states[256] = {0};
static _Bool resized = 0;
static HINSTANCE hinstance;
static WNDCLASSEX wc = {0};
static HWND hwnd;
static HDC hdc;
static float width = 960.0f;
static float height = 540.0f;
static float old_width;
static float old_height;
static float aspect_ratio;


int32_t CALLBACK WinMain(
    HINSTANCE hinstance,
    HINSTANCE h_prev_instance,
    LPSTR lp_cmd_line,
    int32_t n_cmd_show 
) {
    check_resources();
    create_window();
    hdc = GetDC(hwnd);
    HGLRC temp_context = create_temp_context();
    enable_vsync();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        #ifdef demidebug
        fatal(__LINE__, __FILE__, "could not initialzie glew");
        #endif
        win32_err(err_glew_initialization);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
        return -1;
    }

    HGLRC hglrc = create_context();
    wglDeleteContext(temp_context);
    
    float vertex_positions[16] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 1.0f, 0.0f,
        100.0f, 100.0f, 1.0f, 1.0f,
        0.0f, 100.0f, 0.0f, 1.0f
    };
    uint32_t vertex_indecies[6] = {
        0, 1, 2,
        2, 3, 0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vertex_array_object vao = vao_create();
    vertex_buffer_layout layout = vao_create_layout();

    vertex_buffer vb = vb_create(vertex_positions, 4 * 4 * sizeof(float));
    index_buffer_object ibo = ibo_create(vertex_indecies, 6);

    vao_add_element(&layout, 2, GL_FLOAT, sizeof(float), 0);   
    vao_add_element(&layout, 2, GL_FLOAT, sizeof(float), 0);
    vao_add_buffer(&vb, &layout, &vao);
    
    shader shader = shader_create(
        "..\\resources\\shaders\\vertex_shader.shader", 
        "..\\resources\\shaders\\fragment_shader.shader"
    );

    texture texture = texture_create("..\\resources\\textures\\test.png");
    texture_bind(0, &texture);

    float* projection = calloc(16, sizeof(float));
    float* model = calloc(16, sizeof(float));
    float* translate = calloc(16, sizeof(float));
    float* mvp = calloc(16, sizeof(float));
    if (!projection || !model || !translate || !mvp) {
        win32_err(err_allocation_failed);
        goto cleanup;
    }

    unbind_all();
    #ifdef demidebug
    check_gl_errors();
    info(__LINE__, __FILE__, "program enters while loop");
    #endif
    MSG msg;
    _Bool running = 1;
    uint16_t player_x = 0;
    uint16_t player_y = 0;

    aspect_ratio = width / height;
    if (aspect_ratio >= 1.0f)
        orthographic_matrix(projection, 0.0f, width, 0.0f, width / aspect_ratio, -1.0f, 1.0f);
    else
        orthographic_matrix(projection, 0.0f, height * aspect_ratio, 0.0f, height, -1.0f, 1.0f);
    glViewport(0, 0, width, height);

    while (running) {
        if (key_states['W'] && player_y < height - 100.0f) player_y += 5;
        if (key_states['S'] && player_y > 0) player_y -= 5;
        if (key_states['D'] && player_x < width - 100.0f) player_x += 5;
        if (key_states['A'] && player_x > 0) player_x -= 5;

        if (resized) {
            aspect_ratio = width / height;
            if (aspect_ratio >= 1.0f)
                orthographic_matrix(projection, 0.0f, width, 0.0f, width / aspect_ratio, -1.0f, 1.0f);
            else
                orthographic_matrix(projection, 0.0f, height * aspect_ratio, 0.0f, height, -1.0f, 1.0f);
            glViewport(0, 0, width, height);
            resized = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        shader_bind(&shader);
        translate_matrix(model, player_x, player_y, 0);
        multiply_f4x4(mvp, projection, model);
        shader_set_uniformmat4f(&shader, "u_mvp", mvp);
        texture_bind(0, &texture);
        shader_set_uniform1i(&shader, "u_texture", 0);
        vao_bind(&vao);
        ibo_bind(&ibo);
        glDrawElements(GL_TRIANGLES, ibo.count, GL_UNSIGNED_INT, 0);
        SwapBuffers(hdc);
        #ifdef demidebug
        check_gl_errors();
        #endif
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    cleanup:
    free(projection);
    free(model);
    free(translate);
    free(mvp);
    vb_delete(&vb);
    ibo_delete(&ibo);
    vao_delete_layout(&layout);
    vao_delete(&vao);
    shader_delete(&shader);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CLOSE: 
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if (wParam < 256)
                key_states[wParam] = 1;
            break;
        case WM_KEYUP:
            if (wParam < 256)
                key_states[wParam] = 0;
            break;
        case WM_SIZE:
            old_width = width;
            old_height = height;
            width = (float)LOWORD(lParam);
            height = (float)HIWORD(lParam);
            if (height == 0) height = 1;
            resized = 1;
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static void unbind_all() {
    vb_unbind();
    ibo_unbind();
    vao_unbind();
    shader_unbind();
    texture_unbind();
}

static void enable_vsync() {
    if (!__wglewSwapIntervalEXT)
        __wglewSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (__wglewSwapIntervalEXT)
        __wglewSwapIntervalEXT(1);
    #ifdef demidebug
    else
        warning(__LINE__, __FILE__, "wglSwapIntervalEXT not supported");
    #endif
}

static void create_window() {
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = wc_class_name;
    wc.hIcon = (HICON)LoadImage(hinstance, application_icon, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    wc.hIconSm = (HICON)LoadImage(hinstance, application_icon, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wc);
    hwnd = CreateWindowEx(
        0, wc_class_name, application_name, 
        WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU, 
        200, 200, width, height, 0, 0, hinstance, 0 
    );
    ShowWindow(hwnd, SW_SHOW);
}

static void check_resources() {
    DWORD attributes = GetFileAttributesA(application_icon);
    if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
        win32_err(err_file_not_found);
    attributes = GetFileAttributesA("..\\resources\\textures\\test.png");
    if (attributes == INVALID_FILE_ATTRIBUTES || attributes & FILE_ATTRIBUTE_DIRECTORY)
        win32_err(err_file_not_found);
}

static HGLRC create_temp_context() {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,  
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    int32_t pixel_format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixel_format, &pfd);
    HGLRC temp_context = wglCreateContext(hdc);
    wglMakeCurrent(hdc, temp_context);
    return temp_context;
}

static HGLRC create_context() {
    const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
    if (!hglrc) {
        #ifdef demidebug
        fatal(__LINE__, __FILE__, "Failed to create OpenGL 3.3+ core profile context");
        #endif
        win32_err(err_opengl_context);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
        ExitProcess(0);
    }
    wglMakeCurrent(hdc, hglrc);
    return hglrc;
}