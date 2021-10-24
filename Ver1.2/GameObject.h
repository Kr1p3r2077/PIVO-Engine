#pragma once
#include "Transform.h"
#include "Model.h"
#include "Material.h"
#include "RigidBody.h"
#include "Component.h"

class GameObject
{
public:
	Model model;
	Material material;
	Transform transform;
	RigidBody rb;
private:
	std::vector<Component*> components;
public:

	template <typename T>
	T* GetComponent() {
		T* t = nullptr;
		for (int i = 0;i < components.size();i++) {
			t = static_cast<T*>(components[i]);
			if (t) {
				return t;
			}
		}
		return t;
	}

	template <typename T>
	void AddComponent(T comp) {
		Component* component_parent = &comp;
		components.push_back(component_parent);
	}

	GameObject() {
		rb.SetParentTransform(transform);
	}

	void SetMaterial(Material& m) {
		material = m;
	}

	void SetModel(Model& m) {
		model = m;
	}

	void Draw() {
		material.Activate();
		glm::mat4 model_matrix = glm::mat4(1);
		model_matrix = glm::translate(model_matrix, transform.position);

		model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

		model_matrix = glm::scale(model_matrix, transform.scale);
		material.GetShader().setUniformMat4("model", model_matrix);
		model.Draw();
	}

	
	GameObject& operator= (const GameObject& a) {
		material = a.material;
		model = a.model;
		transform = a.transform;
		return *this;
	}
};