#ifndef MODEL_HPP
#define MODEL_HPP

#include <assimp/scene.h>
#include "mesh.hpp"

class Model
{
public:
    Model(std::string path);
    Model() = default;

    void Load(std::string path);

    void Draw(Shader &shader);

private:
    std::vector<Mesh> meshes{};
    std::string directory{};
    std::vector<Texture> textured_loaded{};

    void Load_model(std::string path);
    void Process_node(aiNode *node, const aiScene *scene);
    Mesh Process_mesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> Load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};

#endif // MODEL_HPP
