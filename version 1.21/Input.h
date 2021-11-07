#pragma once

namespace Input {
	bool keyboardKeysPressed[255];

	void Startup() {
		for (int i = 0;i < 255;i++) {
			keyboardKeysPressed[i] = false;
		}
	}

	bool GetKeyDown(char code) {
		return keyboardKeysPressed[code];
	}
};