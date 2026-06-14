#include "shader.hpp"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_code{};
    std::string fragment_code{};

    std::ifstream vertex_shader_file{};
    std::ifstream fragment_shader_file{};

    vertex_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    fragment_shader_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);
        std::stringstream vertex_shader_stream{}, fragment_shader_stream{};

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch(std::ifstream::failure &e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }

    const char * vertex_shader_code = vertex_code.c_str();
    const char * fragment_shader_code = fragment_code.c_str();

    unsigned int vertex{}, fragment{};
    int success{};
    char error_log[512]{};

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, error_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FALIED\n" << error_log << "\n";
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, error_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << error_log << '\n';
    }

    id = glad_glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, error_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << error_log << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::Set_bool(const std::string& name, bool value) const
{
    glUniform1i( glGetUniformLocation(id, name.c_str()), static_cast<int>(value) );
}

void Shader::Set_int(const std::string& name, int value) const
{
    glUniform1i( glGetUniformLocation(id, name.c_str()), value );
}

void Shader::Set_float(const std::string& name, float value) const
{
    glUniform1f( glGetUniformLocation(id, name.c_str()), value );
}

void Shader::Set_vec2(const std::string& name, const glm::vec2 vector)
{
    glUniform2fv( glGetUniformLocation(id, name.c_str()), 1, &vector[0] );
}

void Shader::Set_vec2(const std::string& name, int x, int y)
{
    glUniform2f( glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::Set_vec3(const std::string& name, const glm::vec3 vector)
{
    glUniform3fv( glGetUniformLocation(id, name.c_str()), 1, &vector[0] );
}

void Shader::Set_vec3(const std::string& name, int x, int y, int z)
{
    glUniform3f( glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::Set_vec4(const std::string& name, const glm::vec4 vector)
{
    glUniform4fv( glGetUniformLocation(id, name.c_str()), 1, &vector[0] );
}

void Shader::Set_vec4(const std::string& name, int x, int y, int z, int w)
{
    glUniform4f( glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::Set_matrix2(const std::string& name, const glm::mat2 matrix)
{
    glUniformMatrix2fv( glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &matrix[0][0] );
}

void Shader::Set_matrix3(const std::string& name, const glm::mat3 matrix)
{
    glUniformMatrix3fv( glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &matrix[0][0] );
}

void Shader::Set_matrix4(const std::string& name, const glm::mat4 matrix)
{
    glUniformMatrix4fv( glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &matrix[0][0] );
}

