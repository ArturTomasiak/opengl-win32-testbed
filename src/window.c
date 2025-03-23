#include "window.h"

static void enable_vsync();
static void create_window();
static HGLRC create_temp_context();
static HGLRC create_context();

static const char* application_name = "Demi";
static const char* wc_class_name = "Demi";
static HINSTANCE hinstance;
static WNDCLASSEX wc = {0};
static HWND hwnd;
static HDC hdc;

int32_t CALLBACK WinMain(
    HINSTANCE hinstance,
    HINSTANCE h_prev_instance,
    LPSTR lp_cmd_line,
    int32_t n_cmd_show 
) {
    create_window();
    hdc = GetDC(hwnd);
    HGLRC temp_context = create_temp_context();
    enable_vsync();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fatal(__LINE__, __FILE__, "could not initialzie glew");
        win32_err(err_glew_initialization);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
        return -1;
    }

    HGLRC hglrc = create_context();
    wglDeleteContext(temp_context);
    
    float vertex_positions[8] = {
        -0.5f, -0.5f, 
        0.5f,  -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };    
    uint32_t vertex_indecies[6] = {
        0, 1, 2,
        2, 3, 0
    };

    vertex_array_object vao = vao_create();
    vertex_buffer_layout layout = vao_create_layout();

    vertex_buffer vb = vb_create(vertex_positions, 4 * 2 * sizeof(float));
    index_buffer_object ibo = ibo_create(vertex_indecies, 6 * sizeof(uint32_t));

    vao_add_element(&layout, 2, GL_FLOAT, sizeof(float) * 2, 0);
    vao_add_buffer(&vb, &layout, &vao);
    
    shader shader = shader_create(
        "..\\resources\\shaders\\vertex_shader.shader", 
        "..\\resources\\shaders\\fragment_shader.shader"
    );

    vb_unbind();
    ibo_unbind();
    vao_unbind();
    shader_unbind();

    check_gl_errors();
    info(__LINE__, __FILE__, "program enters while loop");
    int i = 0;
    MSG msg;
    _Bool running = 1;
    while (running) {
        glClear(GL_COLOR_BUFFER_BIT);

        shader_bind(&shader);
        shader_set_uniform4f(&shader, "u_color", 0.2f, 0.3f, 0.8f, 1.0f);
        vao_bind(&vao);
        ibo_bind(&ibo);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SwapBuffers(hdc);
        if (i < 1) {
            check_gl_errors();
            i++;
        }
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
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
            if (wParam == 'W')
                Sleep(10000);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static void enable_vsync() {
    if (!__wglewSwapIntervalEXT)
        __wglewSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (__wglewSwapIntervalEXT)
        __wglewSwapIntervalEXT(1);
    else
        warning(__LINE__, __FILE__, "wglSwapIntervalEXT not supported");
}

static void create_window() {
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = wc_class_name;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wc);
    hwnd = CreateWindowEx(
        0, wc_class_name, application_name, 
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
        200, 200, 640, 480, 0, 0, hinstance, 0 
    );
    ShowWindow(hwnd, SW_SHOW);
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
        fatal(__LINE__, __FILE__, "Failed to create OpenGL 3.3+ core profile context");
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
        ExitProcess(0);
    }
    wglMakeCurrent(hdc, hglrc);
    return hglrc;
}