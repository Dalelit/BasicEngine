#include "pch.h"
#include "BEInput.h"
#include "BELogger.h"

void BEInput::KeyDownInput(char key, unsigned int repeatCount)
{
	// skip if already set and created the event
	if (keyState.test(key)) return; // to do: do want to to manage this differently?

	//BELOG_DEBUG("Key down " + KeyToString(key));

	keyState.set(key, true);

	// handle key down events

	if (key == PAUSEMOUSE) mouseInputEnabled = !mouseInputEnabled;
	if (key == PAUSEUPDATES) paused = !paused;

	// to do: only catching load scene events for the moment
	if (key >= '0' && key <= '9')
	{
		keyEvents.push(Event({ key, Event::Type::KEY_DOWN }));
	}
}

void BEInput::KeyUpInput(char key)
{
	keyState.set(key, false);

	// handle key up events
}

std::string BEInput::KeyToString(char key)
{
	std::string result("");

	if (key >= 'A' && key <= 'Z') return result + key;
	if (key >= '0' && key <= '9') return result + key;
	if (key == ' ') return "Space";
	if (key >= VK_F1 && key <= VK_F24) return result + "F" + std::to_string( key - VK_F1 + 1 );

	return "Other";
}

void BEInput::Update(float deltaTime, BECamera& camera)
{
	if (mouseInputEnabled)
	{
		// convert mouse movement to radian * sensitivity
		float scale = DirectX::XM_PI / 180.0f * mouseSensitivity * deltaTime;
		camera.RotateDirection((float)MouseX() * scale, (float)MouseY() * scale);
	}

	float d = moveSpeed * deltaTime;

	if (IsKeyPressed(MOVERIGHT)) camera.Pan(d, 0.0f, 0.0f);
	if (IsKeyPressed(MOVELEFT)) camera.Pan(-d, 0.0f, 0.0f);
	if (IsKeyPressed(MOVEFORWARD)) camera.Pan(0.0f, 0.0f, d);
	if (IsKeyPressed(MOVEBACKWARD)) camera.Pan(0.0f, 0.0f, -d);
	if (IsKeyPressed(MOVEUP)) camera.Pan(0.0f, d, 0.0f);
	if (IsKeyPressed(MOVEDOWN)) camera.Pan(0.0f, -d, 0.0f);

	RawMouseClear();
}
