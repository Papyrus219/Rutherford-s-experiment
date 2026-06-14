#ifndef FLY_CAMERA_H
#define FLY_CAMERA_H

#include "camera.hpp"

class Fly_camera : public Camera
{
public:
    Fly_camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Fly_camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw_, float pitch_);

    void Process_keyboard(Camera_movement direction, float delta_time) override;

    virtual ~Fly_camera() = default;
};

#endif // FLY_CAMERA_H
