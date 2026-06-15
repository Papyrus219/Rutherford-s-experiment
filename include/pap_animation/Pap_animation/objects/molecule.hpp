#ifndef MOLECULE_HPP
#define MOLECULE_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Pap_animation/objects/vertecies.hpp>
#include <queue>

enum class PHASES
{
    PHASE_1,
    PHASE_2,
    PHASE_3
};

struct Molecule
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    glm::mat4 get_Model_Matrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void Update(float delta_time);
    void Change_phase(PHASES new_phase);

    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};

    float phase1_range{};
    float phase1_end_x{};

    float active{};

    unsigned int vao{};
    unsigned int indices_amount{};

    static std::queue<Molecule *> inactive_objects;
private:

    PHASES phase{PHASES::PHASE_1};
};

#endif //MOLECULE_HPP
