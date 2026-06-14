#ifndef APP_HPP
#define APP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Pap_animation/objects/render_object.hpp>
#include <Pap_animation/camera/orbit_camera.hpp>
#include <Pap_animation/model/model.hpp>

class App
{
public:
    void Setup();
    void Start();

private:
    void Setup_opengl();
    void Setup_simulation();

    void Update_logic();
    void Render_frame();

    static void Framebuffer_size_callback(GLFWwindow * window, int width, int heigtht);

    void ProcessInput(GLFWwindow * window);

    static constexpr int  WINDOW_WIDTH{800}, WINDOW_HEIGHT{600};
    static constexpr int OBJECTS_AMOUNT{60};

    GLFWwindow * window{};

    Shader texture_shader{};
    Shader colot_shader{};

    Model dziura{};
    Model siatka{};
    Model golden{};
    Model lancher{};

    unsigned int texture{};

    Orbit_camera camera{glm::vec3(4.55f, 3.3f, 0.0f), 6.0f, 3.0f};


    std::array<Render_object, OBJECTS_AMOUNT> render_objects{};

    float last_frame{};
    float delta_time{};
    float emission_timer{};
    float emission_interval{};

    float start_x{1.6f};
    float end_x{6.3f};
};

#endif // APP_HPP
