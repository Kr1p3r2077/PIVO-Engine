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

	void SetPosition(glm::vec3 pos) {
		position = pos;
	}

	void SetPosition(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}

	void SetRotation(glm::vec3 rot) {
		rotation = rot;
	}

	void SetRotation(float x, float y, float z) {
		rotation.x = x;
		rotation.y = y;
		rotation.z = z;
	}

	void SetScale(glm::vec3 sc) {
		scale = sc;
	}

	void SetScale(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}

	void Rotate(glm::vec3 rot) {
		rotation += rot;
	}

	void Rotate(float x, float y, float z) {
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}

	void Translate(glm::vec3 pos) {
		position += pos;
	}

	void Translate(float x, float y, float z) {
		position.x += x;
		position.y += y;
		position.z += z;
	}

	void MultiplieSize(glm::vec3 sc) {
		scale *= sc;
	}

	void MultiplieSize(float x, float y, float z) {
		scale.x *= x;
		scale.y *= y;
		scale.z *= z;
	}
};