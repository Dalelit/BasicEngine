#include "BECamera.h"
#include "BEMath.h"
#include <algorithm>

using namespace DirectX;

BECamera::BECamera(XMVECTOR _position, XMVECTOR _direction)
{
	position = _position;
	direction = XMVector3Normalize(_direction);
	Recalc();
}

inline void BECamera::Recalc()
{
	right = XMVector3Normalize(XMVector3Cross(direction, {0,1,0,1}));

	up = XMVector3Normalize( XMVector3Cross(right, direction) );

	upScaled = up * viewPortRatio;

	centre = position + (direction * focalLength);

	viewMatrix = XMMatrixLookToRH(position, direction, {0,1,0,1});

	projectionMatrix = XMMatrixPerspectiveRH(viewPortX, viewPortY, 0.5f, 10000.0f);
}

void BECamera::LookAt(XMVECTOR target)
{
	XMVECTOR lookAtPosition = target;
	direction = XMVector3Normalize(target - position);

	Recalc();
}

BECamera::Ray BECamera::RelativeScreenPositionToRay(float x, float y)
{
	Ray r;
	r.origin = centre + x * right + y * upScaled;
	r.direction = XMVector3Normalize(r.origin - position);

	return r;
}

void BECamera::Pan(float _right, float _up, float _forward)
{
	XMVECTOR change = right * _right + up * _up + direction * _forward;
	position += change;

	Recalc();
}

void BECamera::RotateDirectionMouseInput(int x, int y)
{
	// convert mouse movement to radian * sensitivity
	float scale = XM_PI / 180.0f * mouseSensitivity;
	RotateDirection((float)-x * scale, (float)-y * scale);

	// to do: add a bool for inverting the mouse direction?
}

void BECamera::RotateDirection(float yaw, float pitch, float roll)
{
	constexpr float pitchBound = XM_PI / 2.0f - 0.001f;
	mYaw = WrapAngle(mYaw + yaw);
	mPitch = std::clamp(mPitch + pitch, -pitchBound, pitchBound);

	constexpr XMVECTOR forward = {0.0f, 0.0f, -1.0f, 0.0f};

	XMMATRIX m = XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0.0f);
	direction = XMVector3Normalize( XMVector3Transform(forward, m) );

	Recalc();
}

void BECamera::RotatePosition(float yaw, float pitch)
{
	XMMATRIX m = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
	position = XMVector3Transform(position, m);
	direction = XMVector3Transform(direction, m);

	Recalc();
}
