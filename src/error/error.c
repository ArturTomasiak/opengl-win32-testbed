#include "../includes.h"

void win32_err(enum err_type err) {
    char* error;
    switch (err) {
        case err_allocation_failed:
            error = "memory allocation failed";
            break;
        case err_file_not_found:
            error = "missing files in recources folder";
            break;
        case err_glew_initialization:
            error = "could not initialize glew";
            break;
        case err_shader_compilation:
            error = "could not compile shader";
            break;
        case err_opengl_context:
            error = "failed to create opengl context";
            break;
        case err_file_not_png:
            error = "file expected to be png is a different format";
    }
    MessageBox(NULL, error, "Error", MB_ICONERROR | MB_OK);
    ExitProcess(-1);
}

#ifdef demidebug
static const DWORD red    = FOREGROUND_RED | FOREGROUND_INTENSITY;
static const DWORD green  = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const DWORD orange = FOREGROUND_RED | FOREGROUND_GREEN;
static const DWORD pink   = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
static const DWORD reset  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

void fatal(uint32_t line, char* file, char* message) {
    HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("%s %d %s", "line:", line, file);
    SetConsoleTextAttribute(console_output, red);
    printf(" fatal ");
    SetConsoleTextAttribute(console_output, reset);
    printf("%s\n", message);
}

void warning(uint32_t line, char* file, char* message) {
    HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("%s %d %s", "line:", line, file);
    SetConsoleTextAttribute(console_output, pink);
    printf(" warning ");
    SetConsoleTextAttribute(console_output, reset);
    printf("%s\n", message);
}

void info(uint32_t line, char* file, char* message) {
    HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("%s %d %s", "line:", line, file);
    SetConsoleTextAttribute(console_output, green);
    printf(" info ");
    SetConsoleTextAttribute(console_output, reset);
    printf("%s\n", message);
}

char* translate_gl_error(GLenum error) {
    switch (error) {
        case GL_INVALID_ENUM:
            return "invalid enumeration parameter";
        case GL_INVALID_VALUE:
            return "invalid value parameter";
        case GL_INVALID_OPERATION:
            return "invalid operation";
        case GL_STACK_OVERFLOW:
            return "stack overflow";
        case GL_STACK_UNDERFLOW:
            return "stack underflow";
        case GL_OUT_OF_MEMORY:
            return "memory allocation failed";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "writing to incomplete framebuffer";
        default:
            return "unknown code";
    }
}

void check_gl_errors() {
    GLenum error;
    HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);
    while ((error = glGetError())) {
        SetConsoleTextAttribute(console_output, orange);
        printf("[glGetError] ");
        SetConsoleTextAttribute(console_output, reset);
        printf(translate_gl_error(error));
        printf("%s%d%s\n", " (", error, ")");
    }
}

#endif