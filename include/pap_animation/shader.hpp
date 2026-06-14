#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>


class Shader
{
public:
    Shader(const char* vertex_path, const char* fragment_path);

    void Use();

    void Set_bool(const std::string &name, bool value) const;
    void Set_int(const std::string &name, int value) const;
    void Set_float(const std::string &name, float value) const;

    void Set_vec2(const std::string &name, const glm::vec2 vector);
    void Set_vec2(const std::string &name, int x, int y);

    void Set_vec3(const std::string &name, const glm::vec3 vector);
    void Set_vec3(const std::string &name, int x, int y, int z);

    void Set_vec4(const std::string &name, const glm::vec4 vector);
    void Set_vec4(const std::string &name, int x, int y, int z, int w);

    void Set_matrix2(const std::string &name, const glm::mat2 matrix);

    void Set_matrix3(const std::string &name, const glm::mat3 matrix);

    void Set_matrix4(const std::string &name, const glm::mat4 matrix);

    unsigned int id{};
};

#endif // SHADER_HPP
