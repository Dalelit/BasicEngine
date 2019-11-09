#include "BECamera.h"

BECamera::BECamera()
{
	LookAt(Vector3::Zero);
}

void BECamera::LookAt(Vector3 target)
{
	lookAtPosition = target;
	direction = target - position;
	direction.Normalize();

	right = direction.Cross(Vector3::Up);
	right.Normalize();

	up = right.Cross(direction);
	up.Normalize();
	up *= viewPortRatio;

	centre = position + (direction * focalLength);
}

Ray BECamera::RelativeScreenPositionToRay(float x, float y)
{
	Vector3 point = centre + x * right + y * up;
	Vector3 rayDir = (point - position);
	rayDir.Normalize();

	return Ray(position, rayDir);
}

void BECamera::Translate(Vector3 movement)
{
	position += movement;
	centre += movement;
}

inline Vector3 BECamera::WorldToScreen(Vector3 coord)
{
	Vector3 v = {};
	Vector3 ray = position - coord;
	ray.Normalize();

	float t = (centre - coord).Dot(direction) / ray.Dot(direction);

	if (t >= 0.0f)
	{
		v = ray * t + coord; // world space intersection
		v = v - centre;      // view space location
		v = v / viewPort;    // normalised with size of the view port
	}
	v.z = t;                 // distance from location to viewer, negative if behind viewer

	return v;
}

void BECamera::Pan(float x, float y, float z)
{
	position = position + right * x + (right.Cross(direction)) * y + direction * z;
}

void BECamera::RotateDirection(float yaw, float pitch, float roll)
{
	Matrix m = Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	direction = Vector3::Transform(direction, m);
}

void BECamera::RunTestCases()
{
	Vector3 ans;

	ans = WorldToScreen({ 0,0,0 });

	ans = WorldToScreen({ 1,0,0 });

	ans = WorldToScreen({ 0,1,0 });

	return;
}