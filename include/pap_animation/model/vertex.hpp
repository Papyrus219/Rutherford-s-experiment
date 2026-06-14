#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 possition{};
    glm::vec3 normal{};
    glm::vec2 tex_coords{};
};

#endif
