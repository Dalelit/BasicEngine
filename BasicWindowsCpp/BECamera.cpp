#include "BECamera.h"

BECamera::BECamera(XMVECTOR _position, XMVECTOR _direction)
{
	position = _position;
	direction = XMVector3Normalize(_direction);
	Recalc();
}

inline void BECamera::Recalc()
{
	//right = direction.Cross(XMVECTOR::Up);
	right = XMVector3Normalize(XMVector3Cross(direction, {0,1,0,1}));
	//right = XMVECTOR::Up.Cross(direction);
	//right.Normalize();

	//up = right.Cross(direction);
	up = XMVector3Normalize( XMVector3Cross(right, direction) );
	//up = direction.Cross(right);
	//up.Normalize();
	upScaled = up * viewPortRatio;

	centre = position + (direction * focalLength);

	//viewMatrix = XMMatrixLookToLH(position, direction, XMVECTOR::Up);
	viewMatrix = XMMatrixLookToRH(position, direction, {0,1,0,1});

	//projectionMatrix = Matrix::CreatePerspective(2.0f * viewPort.x, 2.0f * viewPort.y, focalLength, 10000.0f);
	//projectionMatrix = Matrix::CreatePerspective(viewPort.x, viewPort.y, 0.5f, 10000.0f);
	projectionMatrix = XMMatrixPerspectiveRH(viewPortX, viewPortY, 0.5f, 10000.0f);

}

void BECamera::LookAt(XMVECTOR target)
{
	XMVECTOR lookAtPosition = target;
	direction = XMVector3Normalize(target - position);
	//direction.Normalize();

	Recalc();
}

BECamera::Ray BECamera::RelativeScreenPositionToRay(float x, float y)
{
	Ray r;
	r.position = centre + x * right + y * upScaled;
	r.direction = XMVector3Normalize(r.position - position);
	//rayDir.Normalize();

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
	//Matrix m = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	XMMATRIX m = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	//direction = XMVECTOR::Transform(direction, m);
	//direction.Normalize(); // to do: is the normalise needed?
	direction = XMVector3Normalize( XMVector3Transform(direction, m) );

	Recalc();
}

void BECamera::RunTestCases()
{
	XMVECTOR ans;

	ans = WorldToScreen({ 0,0,0 });

	ans = WorldToScreen({ 1,0,0 });

	ans = WorldToScreen({ 0,1,0 });

	return;
}