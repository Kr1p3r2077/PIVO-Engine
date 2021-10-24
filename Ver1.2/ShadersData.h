#pragma once
#include "GameObject.h"
#include "Bebra.h"
#include "Engine.h"

namespace Shaders {

	void UpdateShaderData(Shader& sh, glm::vec3 lightPos, float far_plane) {
        sh.setUniformMat4("projection", Engine::projection);
        sh.setUniformMat4("view", Engine::view);
        sh.setUniformVec3("lights[0].position", lightPos);
        sh.setUniformFloat("lights[0].intensity", 10);
        sh.setUniformVec3("lights[0].color", glm::vec3(1));
        sh.setUniformVec3("viewPos", Bebra::camera.Position);
        sh.setUniformInt("depthMap", 1);
        sh.setUniformFloat("far_plane", far_plane);
	}
};