#pragma once
#include "Transform.h"
#include "Model.h"
#include "Material.h"
class GameObject {
public:
	Model model;
	Material material;
	Transform transform;

	GameObject() {

	}

	void SetMaterial(Material& m) {
		material = m;
	}

	void SetModel(Model& m) {
		model = m;
	}

	void SetPosition(glm::vec3 pos) {
		transform.position = pos;
	}

	void SetRotation(glm::vec3 rot) {
		transform.rotation = rot;
	}

	void SetScale(glm::vec3 sc) {
		transform.scale = sc;
	}

	void Draw() {
		material.Activate();
		glm::mat4 model_matrix = glm::mat4(1);
		model_matrix = glm::translate(model_matrix, transform.position);

		model_matrix = glm::rotate(model_matrix, transform.rotation.x, glm::vec3(1, 0, 0));
		model_matrix = glm::rotate(model_matrix, transform.rotation.y, glm::vec3(0, 1, 0));
		model_matrix = glm::rotate(model_matrix, transform.rotation.z, glm::vec3(0, 0, 1));

		model_matrix = glm::scale(model_matrix, transform.scale);
		material.GetShader().setUniformMat4("model", model_matrix);
		model.Draw();
	}
};