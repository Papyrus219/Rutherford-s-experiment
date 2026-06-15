#ifndef APP_HPP
#define APP_HPP

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <Pap_animation/objects/molecule.hpp>
#include <Pap_animation/camera/orbit_camera.hpp>
#include <Pap_animation/model/model.hpp>

struct Character;

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
    void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

    static void Framebuffer_size_callback(GLFWwindow * window, int width, int heigtht);

    void ProcessInput();

    static constexpr int  WINDOW_WIDTH{1200}, WINDOW_HEIGHT{900};
    static constexpr int OBJECTS_AMOUNT{60};

    GLFWwindow * window{};

    Shader texture_shader{};
    Shader colot_shader{};
    Shader text_shader{};

    Model dziura{};
    Model siatka{};
    Model golden{};
    Model lancher{};

    FT_Library ft{};
    FT_Face face{};
    std::map<char, Character> Characters;

    unsigned int texture{};

    Orbit_camera camera{glm::vec3(4.55f, 3.3f, 0.0f), 6.0f, 3.0f};


    bool show_labels{};

    std::array<Molecule, OBJECTS_AMOUNT> molecules{};

    float last_frame{};
    float delta_time{};
    float emission_timer{};
    float emission_interval{};

    float start_x{1.6f};
    float end_x{6.3f};
};

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;
};

#endif // APP_HPP
