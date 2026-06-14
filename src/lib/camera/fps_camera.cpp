#include <Pap_animation/camera/fps_camera.hpp>

FPS_camera::FPS_camera(glm::vec3 possition_, glm::vec3 up_, float yaw_, float pitch_): Camera{possition_, up_, yaw_, pitch_}
{
}

FPS_camera::FPS_camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw_, float pitch_): Camera{pos_x, pos_y, pos_z, up_x, up_y, up_z, yaw_, pitch_}
{

}

void FPS_camera::Process_keyboard(Camera_movement direction, float delta_time)
{
    float velocity = movement_speed * delta_time;

    float curr_y = possition.y;
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
    possition.y = curr_y;
}
