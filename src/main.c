#include "app.h"

int32_t CALLBACK WinMain(
    HINSTANCE hinstance,
    HINSTANCE h_prev_instance,
    LPSTR lp_cmd_line,
    int32_t n_cmd_show 
) {
    const char* application_name = "Demi";
    const char* wc_class_name = "Demi";
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = wc_class_name;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(
        0, wc_class_name, application_name, 
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
        200, 200, 640, 480, 0, 0, hinstance, 0 
    );
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);
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

    enable_vsync();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fatal(__LINE__, __FILE__, "could not initialzie glew");
        win32_err(err_glew_initialization);
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
        return -1;
    }

    const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
    if (!hglrc) {
        fatal(__LINE__, __FILE__, "Failed to create OpenGL 3.3+ core profile context");
        DestroyWindow(hwnd);
        return -1;
    }
    wglDeleteContext(temp_context);
    wglMakeCurrent(hdc, hglrc);

    uint32_t vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
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

    vertex_buffer vb = create_vertex_buffer(vertex_positions, 4 * 2 * sizeof(float));
    index_buffer ib = create_index_buffer(vertex_indecies, 6 * sizeof(uint32_t));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    uint32_t shader = create_shader("..\\resources\\shaders\\vertex_shader.shader", "..\\resources\\shaders\\fragment_shader.shader");

    int location = glGetUniformLocation(shader, "u_color");
    if (location == -1)
        warning(__LINE__, __FILE__, "u_color uniform not found in shader");

    glBindVertexArray(0);
    glUseProgram(0);
    unbind_vertex_buffer();
    unbind_index_buffer();

    check_gl_errors();
    info(__LINE__, __FILE__, "program enters while loop");
    int i = 0;
    MSG msg;
    _Bool running = 1;
    while (running) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);
        glBindVertexArray(vertex_array);
        bind_index_buffer(&ib);

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
    delete_vertex_buffer(&vb);
    delete_index_buffer(&ib);
    glDeleteProgram(shader);
    DestroyWindow(hwnd);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(temp_context);
    ReleaseDC(hwnd, hdc);
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

void enable_vsync() {
    if (!__wglewSwapIntervalEXT)
        __wglewSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (__wglewSwapIntervalEXT)
        __wglewSwapIntervalEXT(1);
    else
        warning(__LINE__, __FILE__, "wglSwapIntervalEXT not supported");
}