#pragma once
#include "BEEntity.h"


using namespace DirectX;
using namespace SimpleMath;

class BECamera : public BEEntity
{
public:

	float viewPortRatio = 2.0f / 3.0f;
	Vector3 viewPort = { 1.0f, viewPortRatio, 1.0f };

	float focalLength = 1.0f; // is this really a focal length?
	float maxDistance = 10000.0f; // to do: need to think about this number.

	Vector3 up = Vector3::Up;

	BECamera(Vector3 _position, Vector3 _direction);
	inline void Recalc();

	void LookAt(Vector3 target);

	inline Vector3 WorldToScreen(Vector3 coord);

	Ray RelativeScreenPositionToRay(float x, float y);

	void Pan(float _right, float _up, float _forward);
	void RotateDirection(float yaw, float pitch, float roll);

	inline bool OveralpsScreen(Vector3 v) { return ((v.x >= -1.0f && v.x <= 1.0f) || // (x or
		                                            (v.y >= -1.0f && v.y <= 1.0f))   // y in the screen)
													&& v.z > 0.0f; }                 // and infront of screen

	void RunTestCases();

private:
	Vector3 direction = Vector3::Forward;
	Vector3 right = Vector3::Right;
	Vector3 upScaled = Vector3::Up;

	Vector3 centre;

	Matrix projectionMatrix;
	Matrix viewMatrix;

};

