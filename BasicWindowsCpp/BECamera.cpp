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
	right.Normalize();

	up = right.Cross(direction);
	up.Normalize();
	upScaled = up * viewPortRatio;

	centre = position + (direction * focalLength);

	viewMatrix = XMMatrixLookToRH(position, direction, Vector3::Up);
	//viewMatrix = XMMatrixLookAtRH(position, centre, Vector3::Up);

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

//inline Vector3 BECamera::WorldToScreen(Vector3 coord)
//{
//	XMMATRIX Transform = XMMatrixMultiply(viewMatrix, projectionMatrix);
//	XMVECTOR Result = ::XMVector3TransformCoord(coord, Transform);
//	return Result;

	//	reminder: use XMVector3Project(coord, 0, 0, 1.0f, this->viewPortRatio, 0.0f, 1.0f, projectionMatrix, viewMatrix, worldMatrix) ?

	// old code of trying to work it out a different way to do it by using a plane intersection.
	//
	//Vector3 v = {};
	//Vector3 toPosition = position - coord;
	////toPosition.Normalize();

	//Vector3 toCentre = centre - coord;
	////toCentre.Normalize();

	////float t = (centre - coord).Dot(direction) / ray.Dot(centre - position);
	//float t = toCentre.Dot(direction) / toPosition.Dot(direction);

	//if (t >= 0.0f)
	//{
	//	v = toPosition * t + coord; // world space intersection
	//	v = v - centre;      // view space location
	//	v = v / viewPort;    // normalised with size of the view port
	//}
	//v.z = t;                 // distance from location to viewer, negative if behind viewer

	//return v;
//}

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