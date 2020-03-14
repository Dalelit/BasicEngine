#pragma once
#include "pch.h"
#include "BECamera.h"

// To do:
//   queue events up for later?
//   proper class for events and different event types?

class BEInput
{
public:

	class Event
	{
	public:
		enum class Type { KEY_DOWN, KEY_UP };

		char key;
		Type type;
	};

	std::queue<Event> keyEvents;

	inline void RawMouseInput(long x, long y) { mouseX += x; mouseY += y; }; // used by windows message to be udpated
	inline void RawMouseClear() { mouseX = 0; mouseY = 0; }; // called at the end of Update

	// used by windows message to be udpated, and has any key event logic
	void KeyDownInput(char key, unsigned int repeatCount);
	void KeyUpInput(char key);

	std::string KeyToString(char key); // helper function to turn key into a string

	inline bool IsKeyPressed(char key) { return keyState.test(key); };
	inline int MouseX() { return mouseX; };
	inline int MouseY() { return mouseY; };

	void Update(float deltaTime, BECamera& camera);

	void Clear();

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

