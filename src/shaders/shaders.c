#include "shaders.h"

static uint32_t shader_compile(uint32_t type, const char* source);
static char* file_content(const char* location);
static int32_t get_uniform_location(shader* shader, char* name);

shader shader_create(const char* vertex_shader_path, const char* fragment_shader_path) {
    char* vertex_shader = file_content(vertex_shader_path);
    char* fragment_shader = file_content(fragment_shader_path);
    shader shader = {0};
    shader.cache = NULL;
    shader.renderer_id = glCreateProgram();
    uint32_t vs = shader_compile(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fs = shader_compile(GL_FRAGMENT_SHADER, fragment_shader);
    if (!vs || !fs)
        return shader;
    glAttachShader(shader.renderer_id, vs);
    glAttachShader(shader.renderer_id, fs);
    glLinkProgram(shader.renderer_id);
    glValidateProgram(shader.renderer_id);
    glDeleteShader(vs);
    glDeleteShader(fs);
    free(vertex_shader);
    free(fragment_shader);
    return shader;
}

void shader_delete(shader* shader) {
    glDeleteProgram(shader->renderer_id);
    shader->renderer_id = 0;
}

void shader_bind(const shader* shader) {
    glUseProgram(shader->renderer_id);
}

void shader_unbind() {
    glUseProgram(0);
}

void shader_set_uniform4f(shader* shader, char* name, float v0, float v1, float v2, float v3) {
    glUniform4f(get_uniform_location(shader, name), v0, v1, v2, v3);
}

static uint32_t shader_compile(uint32_t type, const char* source) {
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
            return 0;
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

static int32_t get_uniform_location(shader* shader, char* name) {
    for (uint32_t i = 0; i < shader->cache_length; i++)
        if (shader->cache[i].name == name)
            return shader->cache[i].location;

    int32_t location = glGetUniformLocation(shader->renderer_id, name);
    if (location == -1)
        warning(__LINE__, __FILE__, "uniform does not exist");
    shader->cache_length++;
    shader->cache = realloc(shader->cache, sizeof(shader_uniform_cache) * shader->cache_length);
    shader->cache[shader->cache_length - 1].name = name;
    shader->cache[shader->cache_length - 1].location = location;
    return location;
}