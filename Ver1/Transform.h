#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform() {
		position = glm::vec3(0);
		scale = glm::vec3(1);
		rotation = glm::vec3(0);
	}

	Transform& operator=(const Transform& tr) {
		position = tr.position;
		rotation = tr.rotation;
		scale = tr.scale;

		return *this;
	}
};