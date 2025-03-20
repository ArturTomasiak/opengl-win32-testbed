#include "shaders.h"

static char* file_content(const char* location) {
    FILE *file_pointer = fopen(location, "rb");
    if (file_pointer == NULL) {
        char* err = "failed to open file at: ";
        char* full_err = malloc(strlen(err) + strlen(location) + 1);
        strcpy(full_err, err);
        strcat(full_err, location);
        fatal(__LINE__, __FILE__, full_err);
        free(full_err);
        win32_err(err_file_not_found);
        return "\0";
    }
    fseek(file_pointer, 0, SEEK_END);
    uint32_t file_size = ftell(file_pointer);
    rewind(file_pointer);
    char *buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        fclose(file_pointer);
        fatal(__LINE__, __FILE__, "memory allocation failed");
        win32_err(err_allocation_failed);
        return "\0";
    }
    size_t read_size = fread(buffer, 1, file_size, file_pointer);
    buffer[read_size] = '\0';
    fclose(file_pointer);
    return buffer;
}

static uint32_t compile_shader(uint32_t type, const char* source) {
    uint32_t id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);
    int32_t result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int32_t length; 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = malloc(length * sizeof(char));
        if (message == NULL){
            fatal(__LINE__, __FILE__, "shader compilation and memory allocation failed");
            win32_err(err_allocation_failed);
            return id;
        }
        glGetShaderInfoLog(id, length, &length, message);
        glDeleteShader(id);
        printf("%s\n%s", "in file: ", source);
        fatal(__LINE__, __FILE__, message);
        free(message);
        win32_err(err_shader_compilation);
    }
    return id;
}

uint32_t create_shader(const char* vertex_shader_path, const char* fragment_shader_path) {
    char* vertex_shader = file_content(vertex_shader_path);
    char* fragment_shader = file_content(fragment_shader_path);
    uint32_t program = glCreateProgram();
    uint32_t vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    free(vertex_shader);
    free(fragment_shader);
    return program;
}