#pragma once
#define LIGHT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {

public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    float intensity;

    glm::vec3 ambient;
    glm::vec3 color;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    Light(glm::vec3 _position, glm::vec3 _color, float _intensity) {
        position = _position;
        color = _color;
        intensity = _intensity;

        direction = glm::vec3(0, 0, 0);
        cutOff = 12.5f;
        outerCutOff = 17.5f;
        specular = glm::vec3(color.x, color.y, color.z) + glm::vec3(0.3,0.3,0.3);
        constant = 1;
        linear = 0.09f;
        quadratic = 0.032f;
    }

    Light() {
        position = glm::vec3(0, 0, 0);
        color = glm::vec3(1, 1, 1);
        intensity = 1;

        direction = glm::vec3(0, 0, 0);
        cutOff = 12.5f;
        outerCutOff = 17.5f;
        specular = glm::vec3(1, 1, 1);
        constant = 1;
        linear = 0.09f;
        quadratic = 0.032f;
    }

    void SetPosition(glm::vec3 pos) {
        position = pos;
    }

    void SetColor(glm::vec3 col) {
        color = col;
    }

    void SetSpecular(glm::vec3 spec) {
        specular = spec;
    }
};