#include <objects/render_object.hpp>
#include <cstdlib>
#include <iostream>

std::queue<Render_object *> Render_object::inactive_objects{};

void Render_object::Update(float delta_time)
{
    if(!active) return;

    position += velocity * delta_time;

    switch(phase)
    {
        case PHASES::PHASE_1:
            if(position.x >= phase1_end_x) Change_phase(PHASES::PHASE_2);;
            break;
        case PHASES::PHASE_2:
            glm::vec2 center{phase1_end_x, 0.0f};
            glm::vec2 pos2d{position.x, position.z};
            float dist = glm::length(pos2d - center);

            if(dist >= 1.7f)
            {
                active = false;
                inactive_objects.push(this);
            }
            break;
    }

}

void Render_object::Change_phase(PHASES new_phase)
{
    switch(new_phase)
    {
        case PHASES::PHASE_1:
            velocity = {4.0f, 0.0f, 0.0f};
            position = {1.6f, 2.3, 0};
            active = true;
            break;
        case PHASES::PHASE_2:
            float speed = 4.0f;
            float angle{};
            if(rand() % 25 < 2)
            {
                angle = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) ) * 2.0f * M_PI;
                velocity = {std::cos(angle) * speed, 0, std::sin(angle) * speed};
            }
            else
            {
                velocity = {speed, 0, 0};
            }

            break;
    }

    phase = new_phase;
}
