#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
enum EasingType {
	Linear, easeInOutCubic, easeInSine, easeOutSine, easeInOutSine
};

float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

class AnimationTrack {
private:
	float startTime;
	float duration;
	float endTime;
	EasingType easing;

public:
	int index;
	StandartBehaviour* connectedScript;
	AnimationTrack() {
		startTime = 0;
		duration = 0;
	}

	AnimationTrack(float duration) {
		startTime = 0;
		this->duration = duration;
	}

	AnimationTrack(float duration, StandartBehaviour& sb) {
		startTime = 0;
		connectedScript = &sb;
		this->duration = duration;
	}

	void SetEasingType(EasingType type) {
		easing = type;
	}

	void Start() {
		startTime = glfwGetTime();
		endTime = startTime + duration;
	}

	void Start(float delay) {
		startTime = glfwGetTime() + delay;
		endTime = startTime + duration;
	}

	/// <summary>
	/// ¬озвращает значение от 0 до 1 по анимации
	/// </summary>
	/// <returns></returns>
	float GetAnimationState() {
		if (glfwGetTime() < startTime) { connectedScript->UpdateAnimationTrack(0); return 0; }
		float t = (glfwGetTime() - startTime) / duration;
		if (t > 1) { connectedScript->UpdateAnimationTrack(1); return 1; }
		t = GetEasing(t);
		connectedScript->UpdateAnimationTrack(t);
		return t;
	}

private:
	float GetEasing(float t) {
		switch (easing) {
		case easeInOutCubic:
			t = easeInOutCubicF(t);
			break;

		case easeInSine:
			t = easeInSineF(t);
			break;

		case easeOutSine:
			t = easeOutSineF(t);
			break;

		case easeInOutSine:
			t = easeInOutSineF(t);
			break;
		}

		return t;
	}

private:
	float easeInOutCubicF(float x) {
		return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
	}

	float easeInSineF(float x) {
		return 1 - cos((x * 3.141529) / 2);
	}

	float easeOutSineF(float x) {
		return sin((x * 3.141529) / 2);
	}

	float easeInOutSineF(float x) {
		return -(cos(3.141529 * x) - 1) / 2;
	}
};