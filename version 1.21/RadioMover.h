#pragma once
#include "StandartBehaviour.h"
#include "GameObject.h"

class RadioMover : public StandartBehaviour {
public:
	RadioMover() {}
	float rotationSpeed = 1;
	AnimationTrack* anim;

	void Start() override {
		parent->transform.position = glm::vec3(0, 0, 0);
		parent->transform.Rotate(glm::vec3(0, 0, 0));
		parent->transform.scale = glm::vec3(1);
	}

	void Update() override
	{
		parent->transform.position = glm::vec3(sin(glfwGetTime()));
		//std::cout << "Update\n";
		if (Input::GetKeyDown('H')) {
			std::cout << "HI!!!\n";
		}
	}

	/*void OnKeyboardButtonDownH() override {
		Bebra::AddAnimationTrack(anim);
		std::cout << Bebra::animTracks[Bebra::animTracks.size() - 1]->index << std::endl;
		anim->Start();
	}*/

	void UpdateAnimationTrack(float value) override {
		parent->transform.position.y = value;
	}
};