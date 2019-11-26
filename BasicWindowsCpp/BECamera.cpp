#include "BECamera.h"

BECamera::BECamera(Vector3 _position, Vector3 _direction)
{
	position = _position;
	direction = _direction;
	direction.Normalize();
	Recalc();
}

inline void BECamera::Recalc()
{
	right = direction.Cross(Vector3::Up);
	//right = Vector3::Up.Cross(direction);
	right.Normalize();

	up = right.Cross(direction);
	//up = direction.Cross(right);
	up.Normalize();
	upScaled = up * viewPortRatio;

	centre = position + (direction * focalLength);

	//viewMatrix = XMMatrixLookToLH(position, direction, Vector3::Up);
	viewMatrix = XMMatrixLookToRH(position, direction, Vector3::Up);

	//projectionMatrix = Matrix::CreatePerspective(2.0f * viewPort.x, 2.0f * viewPort.y, focalLength, 10000.0f);
	projectionMatrix = Matrix::CreatePerspective(viewPort.x, viewPort.y, 0.5f, 10000.0f);

}

void BECamera::LookAt(Vector3 target)
{
	Vector3 lookAtPosition = target;
	direction = target - position;
	direction.Normalize();

	Recalc();
}

Ray BECamera::RelativeScreenPositionToRay(float x, float y)
{
	Vector3 point = centre + x * right + y * upScaled;
	Vector3 rayDir = (point - position);
	rayDir.Normalize();

	return Ray(position, rayDir);
}

void BECamera::Pan(float _right, float _up, float _forward)
{
	Vector3 change = right * _right + up * _up + direction * _forward;
	position += change;

	Recalc();
}

void BECamera::RotateDirection(float yaw, float pitch, float roll)
{
	Matrix m = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	direction = Vector3::Transform(direction, m);
	direction.Normalize(); // to do: is the normalise needed?

	Recalc();
}

void BECamera::RunTestCases()
{
	Vector3 ans;

	ans = WorldToScreen({ 0,0,0 });

	ans = WorldToScreen({ 1,0,0 });

	ans = WorldToScreen({ 0,1,0 });

	return;
}