#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Bebra.h"

namespace Engine {
	glm::mat4 projection = glm::mat4(1);
	glm::mat4 view = glm::mat4(1);
	void UpdateProjection() {
		projection = glm::perspective(glm::radians(Bebra::camera.Zoom), (float)Bebra::SCR_WIDTH / (float)Bebra::SCR_HEIGHT, 0.1f, 100.0f);
	}

	void UpdateView() {
		view = Bebra::camera.GetViewMatrix();
	}
}