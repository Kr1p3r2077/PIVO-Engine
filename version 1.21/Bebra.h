#pragma once
#include "StandartBehaviour.h"
#include "AninmationTrack.h"
#include <vector>

namespace Bebra {
	std::vector<StandartBehaviour*> userScripts;
	std::vector<AnimationTrack*> animTracks;
	std::vector<GameObject*> AllGameObjects;

	void AddGameObject(GameObject* go) {
		AllGameObjects.push_back(go);
	}

#pragma region User_Scripts

	void UserScripts_Update() {

		for (int i = 0;i < userScripts.size();i++) {
			userScripts[i]->Update();
		}

	}

	void UserScripts_Start() {
		for (int i = 0;i < userScripts.size();i++) {
			userScripts[i]->Start();
		}
	}

	void AddUserScript(StandartBehaviour* script, GameObject& go) {
		userScripts.push_back(script);
		userScripts[userScripts.size() - 1]->AttachTo(go);
	}

#pragma endregion

#pragma region Animation_Tracks

	void AnimTracks_Update() {
		for (int i = 0;i < animTracks.size();i++) {
			//std::cout << animTracks[i]->GetAnimationState(0, 5) << " " << i << " " << animTracks[i]->index << std::endl;

			if (animTracks[i]->GetAnimationState() == 1) {
				animTracks.erase(animTracks.begin() + animTracks[i]->index);

				for (int j = 0;j < animTracks.size();j++) {
					animTracks[j]->index = j;
				}
			}
		}
	}

	void AddAnimationTrack(AnimationTrack* track) {
		animTracks.push_back(track);
		track->index = animTracks.size() - 1;
	}

#pragma endregion

		// Константы
	unsigned int SCR_WIDTH = 1280;
	unsigned int SCR_HEIGHT = 720;

	// Камера
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX = (float)SCR_WIDTH / 2.0;
	float lastY = (float)SCR_HEIGHT / 2.0;
	bool firstMouse = true;
};