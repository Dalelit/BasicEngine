#pragma once
#include "BECommon.h"


using namespace DirectX;
using namespace SimpleMath;

class BECamera
{
public:

	float viewPortRatio = 2.0f / 3.0f;
	Vector3 viewPort = { 1.0f, viewPortRatio, 1.0f };

	float focalLength = 1.0f;

	Vector3 position = { 0.0f, 0.0f, 5.0f };
	Vector3 up = Vector3::Up;

	//Vector3 up = Vector3::Up; // To Do - stop flipping

	BECamera();

	void LookAt(Vector3 target);

	inline Vector3 WorldToScreen(Vector3 coord);

	Ray RelativeScreenPositionToRay(float x, float y);

	void Translate(Vector3 movement);

	void Pan(float x, float y, float z);
	void RotateDirection(float yaw, float pitch, float roll);

	inline bool OveralpsScreen(Vector3 v) { return ((v.x >= -1.0f && v.x <= 1.0f) || // (x or
		                                            (v.y >= -1.0f && v.y <= 1.0f))   // y in the screen)
													&& v.z > 0.0f; }                 // and infront of screen

	void RunTestCases();

private:
	Vector3 direction;
	Vector3 centre;
	Vector3 lookAtPosition;
	Vector3 right;

};

