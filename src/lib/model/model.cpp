#include <Pap_animation/model/model.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include "../../stb_image.h"

Model::Model::Model(std::string path)
{
    Load_model(path);
}

void Model::Load(std::string path)
{
    Load_model(path);
}

void Model::Draw(Shader& shader)
{
    for(unsigned int i{}; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::Load_model(std::string path)
{
    Assimp::Importer import{};
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << '\n';
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    Process_node(scene->mRootNode, scene);
}

void Model::Process_node(aiNode* node, const aiScene* scene)
{
    for(unsigned int i{}; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(Process_mesh(mesh, scene));
    }

    for(unsigned int i{}; i < node->mNumChildren; i++)
    {
        Process_node(node->mChildren[i], scene);
    }
}

Mesh Model::Process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices{};
    std::vector<unsigned int> indices{};
    std::vector<Texture> textures{};

    for(unsigned int i{}; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};

        glm::vec3 vector{};

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.possition = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec{};
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
        {
            vertex.tex_coords = glm::vec2{0.0f, 0.0f};
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i{}; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j{}; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse_maps = Load_material_textures(material, aiTextureType_DIFFUSE, "texture.diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture> specular_maps = Load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh{vertices, indices, textures};
}

unsigned int Texture_from_file(std::string name, std::string directory);

std::vector<Texture> Model::Load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;
    for(unsigned int i{}; i < mat->GetTextureCount(type); i++)
    {
        aiString str{};
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j{}; j < textured_loaded.size(); j++)
        {
            if(std::strcmp(textured_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textured_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture texture;
            std::string filename = std::filesystem::path(str.C_Str()).filename().string();
            texture.id = Texture_from_file(filename, directory);
            texture.path = filename;
            texture.type = type_name;
            textures.push_back(texture);
            textured_loaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int Texture_from_file(std::string name, std::string directory)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data{};
    int width{}, height{}, nr_chanels{};
    data =  stbi_load(std::filesystem::path{directory + "/" + name }.c_str(), &width, &height, &nr_chanels, 0);
    if(data)
    {
        GLenum format{};
        switch(nr_chanels)
        {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture.\n";
    }
   stbi_image_free(data);

    return texture;
}
