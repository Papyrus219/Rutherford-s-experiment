#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "./vertex.hpp"
#include "./texture.hpp"
#include "../shader.hpp"

class Mesh
{
public:
    Mesh(std::vector<Vertex> verticies_, std::vector<unsigned int> indices_, std::vector<Texture> textures_);

    void Draw(Shader &shader);

    std::vector<Vertex> verticies{};
    std::vector<unsigned int> indices{};
    std::vector<Texture> textures{};

private:
    unsigned int vao{}, vbo{}, ebo{};

    void Setup_mesh();
};

#endif
