#include <Pap_animation/objects/render_object.hpp>
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
            if(position.x >= phase1_end_x)
            {
                Change_phase(PHASES::PHASE_2);;
            }
            break;
        case PHASES::PHASE_2:
            {
                glm::vec2 center{phase1_end_x, 0.0f};
                glm::vec2 pos2d{position.x, position.z};
                float dist = glm::length(pos2d - center);

                if(dist >= 1.75f)
                {
                    Change_phase(PHASES::PHASE_3);
                }
            }
            break;
        case PHASES::PHASE_3:
            if(scale.y < 3)
            {
                scale.y += 4.0f * delta_time;
            }
            else
            {
                scale.y = 0.3;
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
        {
            float speed = 4.0f;

            if(rand() % 25 < 2)
            {
                constexpr float spread = glm::radians(60.0f);

                float base_angle = (rand() % 2 == 0) ? std::numbers::pi / 2.0f : -std::numbers::pi / 2.0f; // +90° lub -90°
                float offset = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f) * spread;
                float angle = base_angle + offset;

                velocity = {std::cos(angle) * speed, 0.0f, std::sin(angle) * speed};
            }
            else
            {
                velocity = {speed, 0.0f, 0.0f};
            }
            break;
        }
        case PHASES::PHASE_3:
            velocity = {};
            break;
    }

    phase = new_phase;
}
