#pragma once

namespace Time{

	float lastFrame = 0.0f;
	float deltaTime;
	float currentFrame;


	void Update() {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
};