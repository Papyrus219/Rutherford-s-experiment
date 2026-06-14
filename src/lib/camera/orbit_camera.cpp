#include <camera/orbit_camera.hpp>

Orbit_camera::Orbit_camera(glm::vec3 target_point_, float radius, float height_) : radius(radius), target_point{target_point_}, current_angle(0.0f), height{height_}
{
    Update_orbit();
}

void Orbit_camera::Update_orbit()
{
    float x = glm::sin(current_angle) * radius;
    float z = glm::cos(current_angle) * radius;
    possition = target_point + glm::vec3(x, height, z);
    front = glm::normalize(target_point - possition);

    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
}

void Orbit_camera::Process_keyboard(Camera_movement direction, float delta_time)
{
    if (direction == Camera_movement::LEFT)  current_angle += 1.0f * delta_time;
    if (direction == Camera_movement::RIGHT) current_angle -= 1.0f * delta_time;
    if (direction == Camera_movement::BACKWARD) zoom += 6.0f * delta_time;
    if (direction == Camera_movement::FORWARD) zoom -= 6.0f * delta_time;

    if(zoom < 30.0f) zoom = 30.0f;
    if(zoom > 45.0f) zoom = 45.0f;

    Update_orbit();
}
