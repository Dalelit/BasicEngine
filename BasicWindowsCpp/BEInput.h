#pragma once
#include <bitset>
#include "BECamera.h"

// to do: queue events up for later?

class BEInput
{
public:

	inline void RawMouseInput(long x, long y) { mouseX += x; mouseY += y; }; // used by windows message to be udpated
	inline void RawMouseClear() { mouseX = 0; mouseY = 0; }; // called at the end of Update

	void KeyDownInput(char key); // used by windows message to be udpated, and has any key event logic
	void KeyUpInput(char key);   // used by windows message to be udpated, and has any key event logic

	inline bool IsKeyPressed(char key) { return keyState.test(key); };
	inline int MouseX() { return mouseX; };
	inline int MouseY() { return mouseY; };

	void Update(float deltaTime, BECamera& camera);

	float mouseSensitivity = -2.0f; // speed to rotate the camera * deltatime. Negative to invert it.
	float moveSpeed = 3.0f;			// move speed * deltatime

	char MOVEFORWARD = 'W';
	char MOVEBACKWARD = 'S';
	char MOVERIGHT = 'D';
	char MOVELEFT = 'A';
	char MOVEUP = 'E';
	char MOVEDOWN = 'Q';

	char PAUSEUPDATES = ' ';
	bool paused = false;

	char PAUSEMOUSE = 'M';
	bool mouseInputEnabled = true;

protected:
	int mouseX = 0;
	int mouseY = 0;
	std::bitset<256u> keyState;

};

