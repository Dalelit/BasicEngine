#include "BECamera.h"

BECamera::BECamera()
{
	LookAt(Vector3::Zero);
}

void BECamera::LookAt(Vector3 target)
{
	direction = target - position;
	direction.Normalize();

	centre = position + (direction * focalLength);
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

void BECamera::RunTestCases()
{
	Vector3 ans;

	ans = WorldToScreen({ 0,0,0 });

	ans = WorldToScreen({ 1,0,0 });

	ans = WorldToScreen({ 0,1,0 });

	return;
}