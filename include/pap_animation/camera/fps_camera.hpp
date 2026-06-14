#ifndef FPS_CAMERA_HPP
#define FPS_CAMERA_HPP

#include "camera.hpp"

class FPS_camera : public Camera
{
public:
    FPS_camera(glm::vec3 possition_ = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 up_ = glm::vec3{0.0f, 1.0f, 0.0f}, float yaw_ = YAW, float pitch_ = PITCH);
    FPS_camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw_, float pitch_);

    void Process_keyboard(Camera_movement direction, float delta_time) override;
};

#endif // FPS_CAMERA_HPP
