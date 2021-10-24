#pragma once
#include "Input.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void renderScene();
void renderCube();


#pragma region Callbacks

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании кнопки мыши в данном кадре и соответствующая обработка данных событий
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Bebra::camera.ProcessKeyboard(FORWARD, Time::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Bebra::camera.ProcessKeyboard(BACKWARD, Time::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Bebra::camera.ProcessKeyboard(LEFT, Time::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Bebra::camera.ProcessKeyboard(RIGHT, Time::deltaTime);

    if ((glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) && Input::HPressed == false) {
        Input::HPressed = true;
        for (int i = 0;i < Bebra::userScripts.size();i++) {
            Bebra::userScripts[0]->OnKeyboardButtonDownH();
        }
    }

    if ((glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) && Input::HPressed == true) {
        Input::HPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        Bebra::userScripts[0]->OnKeyboardButtonDownG();
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Engine::projection = glm::perspective(glm::radians(Bebra::camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
    width / 2.0f;
    height / 2.0f;
    Bebra::SCR_WIDTH = width;
    Bebra::SCR_HEIGHT = height;
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
    glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Bebra::firstMouse)
    {
        Bebra::lastX = xpos;
        Bebra::lastY = ypos;
        Bebra::firstMouse = false;
    }

    float xoffset = xpos - Bebra::lastX;
    float yoffset = Bebra::lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    Bebra::lastX = xpos;
    Bebra::lastY = ypos;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        Bebra::camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Bebra::camera.ProcessMouseScroll(yoffset);
}


#pragma endregion