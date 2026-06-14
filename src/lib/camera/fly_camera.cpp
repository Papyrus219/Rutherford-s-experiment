#include <Pap_animation/camera/fly_camera.hpp>

Fly_camera::Fly_camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch): Camera{position, up, yaw, pitch}
{

}

Fly_camera::Fly_camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw_, float pitch_): Camera{pos_x,pos_y, pos_z, up_x, up_y, up_z, yaw_, pitch_}
{

}

void Fly_camera::Process_keyboard(Camera_movement direction, float delta_time)
{
    float velocity = movement_speed * delta_time;
    switch(direction)
    {
        case Camera_movement::FORWARD:
            possition += front * velocity;
            break;
        case Camera_movement::BACKWARD:
            possition -= front * velocity;
            break;
        case Camera_movement::LEFT:
            possition -= right * velocity;
            break;
        case Camera_movement::RIGHT:
            possition += right * velocity;
            break;
    }
}
