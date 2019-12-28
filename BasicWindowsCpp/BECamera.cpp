#include "BECamera.h"

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
	r.position = centre + x * right + y * upScaled;
	r.direction = XMVector3Normalize(r.position - position);

	return r;
}

void BECamera::Pan(float _right, float _up, float _forward)
{
	XMVECTOR change = right * _right + up * _up + direction * _forward;
	position += change;

	Recalc();
}

void BECamera::RotateDirection(float yaw, float pitch, float roll)
{
	XMMATRIX m = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	direction = XMVector3Normalize( XMVector3Transform(direction, m) );

	Recalc();
}

void BECamera::RotatePosition(float yaw, float pitch)
{
	XMMATRIX m = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
	position = XMVector3Transform(position, m);
	direction = XMVector3Transform(direction, m);

	Recalc();
}
