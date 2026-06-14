#define STB_IMAGE_IMPLEMENTATION
#include <Pap_animation/app.hpp>
#include <stb_image.h>

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

    ProcessInput(window);

    for(auto & object : render_objects)
    {
        object.Update(delta_time);
    }

    emission_timer += delta_time;

    if(emission_timer >= emission_interval)
    {
        emission_timer -= emission_interval;
        if(!Render_object::inactive_objects.empty())
        {
            Render_object::inactive_objects.front()->active = true;
            Render_object::inactive_objects.front()->Change_phase(PHASES::PHASE_1);
            Render_object::inactive_objects.pop();
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
    for(auto & render_object : render_objects)
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

    glfwSwapBuffers(window);
    glfwPollEvents();
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

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texture_shader.Load("../../shaders/texture_vertex.glsl", "../../shaders/texture_fragment.glsl");
    colot_shader.Load("../../shaders/texture_vertex.glsl", "../../shaders/color_fragment.glsl");

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

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void App::Setup_simulation()
{
    std::srand(std::time(NULL));

    float range = end_x - start_x;

    float travel_time = (end_x - start_x + 1.7f) / 4.0f;
    emission_interval = travel_time / 20;

    Setup_sphere_vertecies(50, 50, 0.1);
    Setup_vaos();
    for(int i = 0; i < render_objects.size(); i++)
    {
        render_objects[i].vao = sphere_VAO;
        render_objects[i].indices_amount = sphere_indices_amount;
        render_objects[i].position = {1.6f, 2.3f, 0.0f};
        render_objects[i].scale = {0.3f, 0.3f, 0.3f};
        render_objects[i].velocity = {4.0f, 0.0f, 0.0f};
        render_objects[i].phase1_end_x = end_x;
        render_objects[i].phase1_range = range;
        render_objects[i].active = false;
        Render_object::inactive_objects.push(&render_objects[i]);
    }
}


void App::Framebuffer_size_callback(GLFWwindow * window, int width, int heigtht)
{
    glViewport(0, 0, width, heigtht);
}

void App::ProcessInput(GLFWwindow* window)
{
    float cameraSpeed = 2.0f * delta_time;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT))
    {
        camera.Process_keyboard(Camera_movement::RIGHT, cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        camera.Process_keyboard(Camera_movement::LEFT, cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_UP))
    {
        camera.Process_keyboard(Camera_movement::FORWARD, cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN))
    {
        camera.Process_keyboard(Camera_movement::BACKWARD, cameraSpeed);
    }

}
