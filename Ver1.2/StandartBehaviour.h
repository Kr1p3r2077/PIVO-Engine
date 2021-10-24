#pragma once
#include "GameObject.h"
class StandartBehaviour {
public:
	StandartBehaviour(){}

	GameObject* parent;

	virtual void Start() {}

	virtual void Update() { std::cout << "Update On Standart Behaviour\n"; }

	virtual void AttachTo(GameObject& go)
	{
		parent = &go;
	}

	virtual void UpdateAnimationTrack(float value) {}

	virtual void OnKeyboardButtonDownH(){}

	virtual void OnKeyboardButtonDownG(){}
};