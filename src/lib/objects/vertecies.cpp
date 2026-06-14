#include <numeric>
#include <cmath>
#include <objects/vertecies.hpp>
#include <glad/glad.h>

extern const std::vector<Vertex> cube_vertices;
extern const std::vector<int> cube_indices;
std::vector<Vertex> sphere_vertices{};
std::vector<int> sphere_indices{};

unsigned int cube_VAO{};
unsigned int cube_VBO{};
unsigned int cube_EBO{};
unsigned int cube_indices_amount{};

unsigned int sphere_VAO;
unsigned int sphere_VBO;
unsigned int sphere_EBO;
unsigned int sphere_indices_amount;


void Setup_vaos()
{
    glGenBuffers(1, &cube_VBO);
    glGenBuffers(1, &cube_EBO);

    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(Vertex), cube_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_indices.size() * sizeof(int), cube_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);


    glGenBuffers(1, &sphere_VBO);
    glGenBuffers(1, &sphere_EBO);

    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(Vertex), sphere_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_indices.size() * sizeof(int), sphere_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);


    cube_indices_amount = cube_indices.size();
    sphere_indices_amount = sphere_indices.size();

    glBindVertexArray(0);
}

void Setup_sphere_vertecies(int stack_count, int sector_count, float radius)
{
    sphere_vertices.clear();
    sphere_indices.clear();


    float xy{}, length_inv{1.0f / radius};
    glm::vec3 pos{};
    glm::vec3 norm{};
    glm::vec2 tex_pos{};

    float sector_step = 2 * std::numbers::pi / sector_count;
    float stack_step = std::numbers::pi / stack_count;
    float sector_angle{}, stack_angle{};

    for(int i = 0; i <= stack_count; i++)
    {
        stack_angle = std::numbers::pi / 2 - i * stack_step;
        xy = radius * std::cos(stack_angle);
        pos.z = radius * std::sin(stack_angle);

        for(int j = 0; j <= sector_count; j++)
        {
            sector_angle = j * sector_step;

            pos.x = xy * std::cos(sector_angle);
            pos.y = xy * std::sin(sector_angle);

            norm.x = pos.x * length_inv;
            norm.y = pos.y * length_inv;
            norm.z = pos.z * length_inv;

            tex_pos.x = static_cast<float>(j) / sector_step;
            tex_pos.y = static_cast<float>(i) / stack_step;

            sphere_vertices.push_back({pos, norm, tex_pos});
        }
    }

    int k1, k2;
    for(int i = 0; i < stack_count; ++i)
    {
        k1 = i * (sector_count + 1);     // beginning of current stack
        k2 = k1 + sector_count + 1;      // beginning of next stack

        for(int j = 0; j < sector_count; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                sphere_indices.push_back(k1);
                sphere_indices.push_back(k2);
                sphere_indices.push_back(k1 + 1);
            }

            if(i != (stack_count-1))
            {
                sphere_indices.push_back(k1 + 1);
                sphere_indices.push_back(k2);
                sphere_indices.push_back(k2 + 1);
            }
        }
    }
}

const std::vector<Vertex> cube_vertices = {
    {{-0.5f, -0.5f, -0.5f},  {0.0, 0.0, -1.0},  {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f},  {0.0, 0.0, -1.0},  {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f},  {0.0, 0.0, -1.0},  {1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f},  {0.0, 0.0, -1.0},  {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f},  {0.0, 0.0, -1.0},  {0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f},  {0.0, 0.0, -1.0},  {0.0f, 0.0f}},

    {{-0.5f, -0.5f,  0.5f},  {0.0, 0.0, 1.0},   {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f},  {0.0, 0.0, 1.0},   {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f},  {0.0, 0.0, 1.0},   {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f},  {0.0, 0.0, 1.0},   {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f},  {0.0, 0.0, 1.0},   {0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f},  {0.0, 0.0, 1.0},   {0.0f, 0.0f}},

    {{-0.5f,  0.5f,  0.5f},  {-1.0, 0.0, 0.0},  {1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f},  {-1.0, 0.0, 0.0},  {1.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f},  {-1.0, 0.0, 0.0},  {0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f},  {-1.0, 0.0, 0.0},  {0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f},  {-1.0, 0.0, 0.0},  {0.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f},  {-1.0, 0.0, 0.0},  {1.0f, 0.0f}},

    {{ 0.5f,  0.5f,  0.5f},  {1.0, 0.0, 0.0},   {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f},  {1.0, 0.0, 0.0},   {1.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f},  {1.0, 0.0, 0.0},   {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f},  {1.0, 0.0, 0.0},   {0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f},  {1.0, 0.0, 0.0},   {0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f},  {1.0, 0.0, 0.0},   {1.0f, 0.0f}},

    {{-0.5f, -0.5f, -0.5f},  {0.0, -1.0, 0.0},  {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f},  {0.0, -1.0, 0.0},  {1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f},  {0.0, -1.0, 0.0},  {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f},  {0.0, -1.0, 0.0},  {1.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f},  {0.0, -1.0, 0.0},  {0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f},  {0.0, -1.0, 0.0},  {0.0f, 1.0f}},


    {{-0.5f,  0.5f, -0.5f},  {0.0, 1.0, 0.0},   {0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f},  {0.0, 1.0, 0.0},   {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f},  {0.0, 1.0, 0.0},   {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f},  {0.0, 1.0, 0.0},   {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f},  {0.0, 1.0, 0.0},   {0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f},  {0.0, 1.0, 0.0},   {0.0f, 1.0f}}
};

const std::vector<int> cube_indices = {
    0,  1,  2,  3,  4,  5,
    6,  7,  8,  9,  10, 11,
    12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};
