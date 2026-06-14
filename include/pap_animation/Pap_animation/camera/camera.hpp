#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:

    Camera(glm::vec3 possition_ = glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3 up_ = glm::vec3{0.0f, 1.0f, 0.0f}, float yaw_ = YAW, float pitch_ = PITCH);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw_, float pitch_);

    glm::mat4 Get_view_matrix() { return glm::lookAt(possition, possition + front, up); };
    float Get_fov() { return zoom;};

    virtual void Process_keyboard(Camera_movement direction, float delta_time) = 0;
    void Process_mouse_scroll(float y_offset);
    void Process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch = true);

    virtual ~Camera() = default;
protected:
    void Update_camera_vectors();

    glm::vec3 possition{};
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 world_up{};

    float yaw{};
    float pitch{};
    float movement_speed{};
    float mouse_sensivity{};
    float zoom{};

    //Defaults
    static constexpr float YAW{-90.0f};
    static constexpr float PITCH{};
    static constexpr float SPEED{2.5f};
    static constexpr float SENITIVITY{0.1f};
    static constexpr float ZOOM{45.0f};
};

#endif // CAMERA_HPP
