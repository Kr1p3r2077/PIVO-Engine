#pragma once
#include "Bebra.h"

namespace Gameobjects {
	Model radio, plane;
	GameObject go, floor;

	void DrawAll() {
		for (int i = 0;i < Bebra::AllGameObjects.size();i++) {
			Bebra::AllGameObjects[i]->Draw();
		}
		//go.Draw();
		//floor.Draw();
	}

	void UpdatePhysics() {
		go.rb.MoveObjectToVelocityComposition();
	}
}