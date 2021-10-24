#pragma once
#include "Transform.h"
#include "Component.h"

class RigidBody : public Component {
public:
	float weight;
	bool is_static;
	glm::vec3 velocity;
	glm::vec3 gravity;
	Transform* trans = nullptr;

	RigidBody() {
		weight = 1;
		is_static = false;
		velocity = glm::vec3(0);
		gravity = glm::vec3(0, -10, 0);
	}

	void SetParentTransform(Transform& t) {
		trans = &t;
	}

	void SetVelocity(glm::vec3 vel) {
		velocity = vel;
	}

	void MoveObjectToVelocityComposition() {
		if (!is_static) {
			trans->Translate(velocity * Time::deltaTime);
		}
	}

private:
	void ResistanceCalculation() {
		velocity += gravity;
	}
};