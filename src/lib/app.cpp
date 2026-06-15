#define STB_IMAGE_IMPLEMENTATION
#include <ft2build.h>
#include <stb_image.h>
#include <Pap_animation/app.hpp>

void App::Setup()
{
    Setup_opengl();
    Setup_simulation();
}

void App::Start()
{
    last_frame = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        Update_logic();

        Render_frame();
    }

    glfwTerminate();
}

void App::Update_logic()
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    ProcessInput();

    for(auto & object : molecules)
    {
        object.Update(delta_time);
    }

    emission_timer += delta_time;

    if(emission_timer >= emission_interval)
    {
        emission_timer -= emission_interval;
        if(!Molecule::inactive_objects.empty())
        {
            Molecule::inactive_objects.front()->active = true;
            Molecule::inactive_objects.front()->Change_phase(PHASES::PHASE_1);
            Molecule::inactive_objects.pop();
        }
    }
}

void App::Render_frame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    texture_shader.Use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Get_fov()), 800.0f / 600.0f, 0.1f, 100.0f);
    texture_shader.Set_matrix4("view", camera.Get_view_matrix());
    texture_shader.Set_matrix4("projection", projection);

    colot_shader.Use();
    for(auto & render_object : molecules)
    {
        if(!render_object.active) continue;

        colot_shader.Set_matrix4("model", render_object.get_Model_Matrix());
        colot_shader.Set_matrix4("view", camera.Get_view_matrix());
        colot_shader.Set_matrix4("projection", projection);
        colot_shader.Set_vec4("color", {0.7f, 0.7f, 1.0f, 1.0f});
        colot_shader.Set_bool("is_glowing", true);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(render_object.vao);
        glDrawElements(GL_TRIANGLES, render_object.indices_amount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glm::mat4 model{};

    colot_shader.Use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, {4.3f, 2.3f, 0.3f});
    model = glm::scale(model, {2.0f, 2.0f, 2.0f});

    model = glm::rotate(model, glm::radians(16.5f), glm::vec3(0.0f, 1.0f, 0.0f));
    colot_shader.Set_matrix4("model", model);
    colot_shader.Set_matrix4("view", camera.Get_view_matrix());
    colot_shader.Set_matrix4("projection", projection);
    colot_shader.Set_vec4("color", {0.0176f, 0.486f, 0.145f, 1.0f});
    colot_shader.Set_bool("is_glowing", false);

    siatka.Draw(colot_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, {2.0f, 2.3f, 0.0f});
    model = glm::scale(model, {0.5f, 0.5f, 0.5f});
    colot_shader.Set_matrix4("model", model);
    colot_shader.Set_matrix4("view", camera.Get_view_matrix());
    colot_shader.Set_matrix4("projection", projection);
    colot_shader.Set_vec4("color", {0.0f, 0.0f, 0.345f, 0.5f});
    colot_shader.Set_bool("is_glowing", false);

    glDepthMask(GL_FALSE);
    lancher.Draw(colot_shader);
    glDepthMask(GL_TRUE);

    texture_shader.Use();

    model = glm::mat4(1.0f);
    model = glm::translate(model, {4.65f, 1.7, 0.0f});
    model = glm::scale(model, {0.5f, 0.5f, 0.5f});
    texture_shader.Set_matrix4("model", model);
    texture_shader.Set_matrix4("view", camera.Get_view_matrix());
    texture_shader.Set_matrix4("projection", projection);

    dziura.Draw(texture_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, {6.3f, 2.3f, 0.0f});
    model = glm::scale(model, {0.3f, 0.5f, 0.5f});
    texture_shader.Set_matrix4("model", model);
    texture_shader.Set_matrix4("view", camera.Get_view_matrix());
    texture_shader.Set_matrix4("projection", projection);

    golden.Draw(texture_shader);

    if(show_labels)
    {
        text_shader.Use();

        glm::mat4 textProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

        text_shader.Set_matrix4("projection", textProjection);

        glDisable(GL_DEPTH_TEST);
        glm::vec4 clip = projection * camera.Get_view_matrix() * glm::vec4(6.5f, 2.3f, -0.5f, 1.0f);
        clip /= clip.w;

        float screenX = (clip.x * 0.5f + 0.5f) * 800.0f;
        float screenY = (clip.y * 0.5f + 0.5f) * 600.0f;

        RenderText(text_shader, "Golden foil", screenX, screenY, 0.5f,
                glm::vec3(1.0, 1.0f, 1.0f));

        clip = projection * camera.Get_view_matrix() * glm::vec4(8.0f, 2.3f, -0.5f, 1.0f);
        clip /= clip.w;

        screenX = (clip.x * 0.5f + 0.5f) * 800.0f;
        screenY = (clip.y * 0.5f + 0.5f) * 600.0f;

        RenderText(text_shader, "Fluororestrictive screen", screenX, screenY, 0.5f,
                glm::vec3(1.0, 1.0f, 1.0f));

        clip = projection * camera.Get_view_matrix() * glm::vec4(2.25f, 2.3f, -0.5f, 1.0f);
        clip /= clip.w;

        screenX = (clip.x * 0.5f + 0.5f) * 800.0f;
        screenY = (clip.y * 0.5f + 0.5f) * 600.0f;

        RenderText(text_shader, "Radiation source", screenX, screenY, 0.5f,
                glm::vec3(1.0, 1.0f, 1.0f));

        for(auto & molecule : molecules)
        {
            if(!molecule.active) continue;
            clip = projection * camera.Get_view_matrix() * glm::vec4(molecule.position, 1.0f);
            clip /= clip.w;

            screenX = (clip.x * 0.5f + 0.5f) * 800.0f;
            screenY = (clip.y * 0.5f + 0.5f) * 600.0f;

            RenderText(text_shader, "alpha particle", screenX, screenY, 0.25f,
                   glm::vec3(1.0, 1.0f, 1.0f));
        }

        glEnable(GL_DEPTH_TEST);

    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void App::RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color)
{
    s.Use();
    glUniform3f(glGetUniformLocation(s.id, "textColor"),
                color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_VAO);
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,
                ypos + h, 0.0f, 0.0f },
                { xpos,
                    ypos,
                    0.0f, 1.0f },
                    { xpos + w, ypos,
                        1.0f, 1.0f },
                        { xpos,
                            ypos + h, 0.0f, 0.0f },
                            { xpos + w, ypos,
                                1.0f, 1.0f },
                                { xpos + w, ypos + h, 1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void App::Setup_opengl()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rutherford Experiment", NULL, NULL);
    if(window == NULL)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        std::exit(EXIT_FAILURE);
    }

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    glViewport(0, 0, framebufferWidth, framebufferHeight);


    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texture_shader.Load("../../shaders/texture_vertex.glsl", "../../shaders/texture_fragment.glsl");
    colot_shader.Load("../../shaders/texture_vertex.glsl", "../../shaders/color_fragment.glsl");
    text_shader.Load("../../shaders/vertex_text.glsl", "../../shaders/fragment_text.glsl");

    dziura.Load("../../objects/dziura/dziura.obj");
    siatka.Load("../../objects/siatka/siatka.obj");
    golden.Load("../../objects/golden/golden.obj");
    lancher.Load("../../objects/lancher/lancher.obj");

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width{}, height{}, nr_channels{};
    unsigned char * data = stbi_load("../../texture/profilowe.png", &width, &height, &nr_channels, 0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture\n";
    }

    stbi_image_free(data);

    glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);

    if(FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";
    }

    if(FT_New_Face(ft, "../../fonts/physic_font.ttf", 0, &face))
    {
        std::cerr << "ERROR::FREETYPE: Failed to load font\n";
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character =
        {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void App::Setup_simulation()
{
    std::srand(std::time(NULL));

    float range = end_x - start_x;

    float travel_time = (end_x - start_x + 1.7f) / 4.0f;
    emission_interval = travel_time / 10;

    Setup_sphere_vertecies(50, 50, 0.1);
    Setup_vaos();
    for(int i = 0; i < molecules.size(); i++)
    {
        molecules[i].vao = sphere_VAO;
        molecules[i].indices_amount = sphere_indices_amount;
        molecules[i].position = {1.6f, 2.3f, 0.0f};
        molecules[i].scale = {0.3f, 0.3f, 0.3f};
        molecules[i].velocity = {4.0f, 0.0f, 0.0f};
        molecules[i].phase1_end_x = end_x;
        molecules[i].phase1_range = range;
        molecules[i].active = false;
        Molecule::inactive_objects.push(&molecules[i]);
    }
}


void App::Framebuffer_size_callback(GLFWwindow * window, int width, int heigtht)
{
    glViewport(0, 0, width, heigtht);
}

void App::ProcessInput()
{
    float cameraSpeed = 2.0f * delta_time;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.Process_keyboard(Camera_movement::RIGHT, cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera.Process_keyboard(Camera_movement::LEFT, cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.Process_keyboard(Camera_movement::FORWARD, cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera.Process_keyboard(Camera_movement::BACKWARD, cameraSpeed);
    }

    static bool space_was_pressed = false;
    bool is_space_pressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (is_space_pressed && !space_was_pressed)
    {
        show_labels = !show_labels;
    }

    space_was_pressed = is_space_pressed;

}
