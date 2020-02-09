#include "BEInput.h"

void BEInput::KeyDownInput(char key)
{
	keyState.set(key, true);

	// handle key down events

	if (key == PAUSEMOUSE) mouseInputEnabled = !mouseInputEnabled;
	if (key == PAUSEUPDATES) paused = !paused;

	// to do: only catching load scene events for the moment
	if (key >= '0' && key <= '4')
	{
		keyEvents.push(Event({ key, Event::Type::KEY_DOWN }));
	}
}

void BEInput::KeyUpInput(char key)
{
	keyState.set(key, false);

	// handle key up events
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
