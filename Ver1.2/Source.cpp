#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Windows.h"

#include "Camera.h"
#include "Model.h"
#include "ShadowsFBO.h"
#include "Material.h"
#include "GameObject.h"
#include "ShadersData.h"
#include "RadioMover.h"
#include "Time.h"
#include "AninmationTrack.h"
#include "Bebra.h"
#include "Callbacks.h"
#include "GameObjects.h"
#include <iostream>

using namespace Time;
using namespace Gameobjects;

float posY = 0;

int main()
{

#pragma region OpenGL_Initialisation

    // glfw: инициализация и конфигурирование
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw: создание окна
    GLFWwindow* window = glfwCreateWindow(Bebra::SCR_WIDTH, Bebra::SCR_HEIGHT, "BEBRA Engine 0.9", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Сообщаем GLFW, чтобы он захватил наш курсор
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);

    // glad: загрузка всех указателей на OpenGL-функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Конфигурирование глобального состояния OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

#pragma endregion

#pragma region Shaders_Initialisation
    Shader shader("Shaders/pbr3.vs", "Shaders/pbr4.fs");
    Shader DepthShaderFromLight("Shaders/point_shadow_depth.vs", "Shaders/point_shadow_depth.fs", "Shaders/point_shadow_depth.gs");
#pragma endregion

#pragma region Material_Initialisation
    Material radioMat;

    radioMat.SetShader(shader);

    radioMat.SetAlbedoColor(glm::vec3(1,1,1), "Textures/radio/albedo.jpg");
    radioMat.SetMetallicValue(1, "Textures/radio/metallic.jpg");
    radioMat.SetRoughnessValue(0,"Textures/radio/roughness.jpg");
    radioMat.SetNormalTexture("Textures/radio/normal.png");
    radioMat.SetEmissionColor(glm::vec3(1),"Textures/radio/emis.jpg");
    radioMat.SetAOTexture("Textures/radio/ao.jpg");
#pragma endregion
    
#pragma region Shadows_Initialisation
    unsigned int depthCubemap, depthMapFBO;
    unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    createShadowFBO(SHADOW_HEIGHT, SHADOW_HEIGHT, &depthMapFBO, &depthCubemap);
#pragma endregion

#pragma region GameObjects_Initialisation_Configuration

    radio.loadModel("Models/Vintage_radio.obj");
    plane.loadModel("Models/plane.obj");

    Gameobjects::floor.SetModel(plane);
    Gameobjects::floor.SetMaterial(radioMat);
    Gameobjects::floor.transform.SetScale(10, 10, 10);
    Gameobjects::floor.transform.SetPosition(0, 0, 0);

    go.SetModel(radio);
    go.SetMaterial(radioMat);
    go.transform.SetPosition(0, 10, 0);

    RigidBody rb;
    rb.weight = 1234;
    go.AddComponent(rb);
    std::cout << go.GetComponent<RigidBody>()->weight;

#pragma endregion


#pragma region ImGui_Initialisation

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

#pragma endregion

    // Параметры освещения
    glm::vec3 lightPos(0, 3, 2);

    RadioMover rm;
    Bebra::AddUserScript(&rm, go);
    Bebra::UserScripts_Start();

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        Time::Update();

        // Обработка ввода
        processInput(window);

        Bebra::UserScripts_Update();
        Bebra::AnimTracks_Update();
        Gameobjects::UpdatePhysics();
        // Рендер
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

#pragma region ShadowProjectionMatrices

        // 0. Создаем матрицы трансформации кубической карты глубины
        float near_plane = 1.0f;
        float far_plane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        // 1. Рендерим сцену в кубическую карту глубины
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        DepthShaderFromLight.use();
        Engine::UpdateProjection();
        Engine::UpdateView();
        Shaders::UpdateShaderData(shader, lightPos, far_plane);
        for (unsigned int i = 0; i < 6; ++i)
            DepthShaderFromLight.setUniformMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        DepthShaderFromLight.setUniformFloat("far_plane", far_plane);
        DepthShaderFromLight.setUniformVec3("lightPos", lightPos);

#pragma endregion

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. Рендерим сцену как обычно
        glViewport(0, 0, Bebra::SCR_WIDTH, Bebra::SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        Engine::UpdateProjection();
        Engine::UpdateView();
        Shaders::UpdateShaderData(shader, lightPos, far_plane);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        renderScene();

        ImGui::Begin("Transform");

        ImGui::SliderFloat("Size", &posY, 0.1, 5);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void renderScene()
{
    Gameobjects::DrawAll();
}