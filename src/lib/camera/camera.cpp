#include <camera/camera.hpp>

Camera::Camera(glm::vec3 possition_, glm::vec3 up_, float yaw_, float pitch_): possition{possition_}, front{0.0f,0.0f, -1.0f}, world_up{up_}, yaw{yaw_}, pitch{pitch_}, movement_speed{SPEED}, mouse_sensivity{SENITIVITY}, zoom{ZOOM}
{
    Update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw_, float pitch_): possition{pos_x, pos_y, pos_z}, front{0.0f, 0.0f, -1.0f}, world_up{up_x, up_y, up_z}, yaw{yaw_}, pitch{pitch_}, movement_speed{SPEED}, mouse_sensivity{SENITIVITY}, zoom{ZOOM}
{
    Update_camera_vectors();
}

void Camera::Process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= mouse_sensivity;
    y_offset *= mouse_sensivity;

    yaw += x_offset;
    pitch += y_offset;

    if(constrain_pitch)
    {
        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;
    }

    Update_camera_vectors();
}


void Camera::Process_mouse_scroll(float y_offset)
{
    zoom -= (float)y_offset;
    if(zoom < 1.0f) zoom = 1.0f;
    if(zoom > 45.0f) zoom = 45.0f;
}

void Camera::Update_camera_vectors()
{
    glm::vec3 front{};
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    right = glm::normalize( glm::cross(this->front, world_up) );
    up = glm::normalize( glm::cross(right, this->front) );
}
