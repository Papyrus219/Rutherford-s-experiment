#ifndef ORBIT_CAMERA_HPP
#define ORBIT_CAMERA_HPP

#include "./camera.hpp"

class Orbit_camera : public Camera
{
public:
    Orbit_camera(glm::vec3 target = glm::vec3(0.0f), float radius = 5.0f, float height = 0.0f);

    void Process_keyboard(Camera_movement direction, float delta_time) override;

    void Set_angle(float angle) { current_angle = angle; Update_orbit(); }
    void Set_target(glm::vec3 new_target) { target_point = new_target; Update_orbit(); }

private:
    void Update_orbit();
    glm::vec3 target_point;
    float radius;
    float height;
    float current_angle;
};

#endif
