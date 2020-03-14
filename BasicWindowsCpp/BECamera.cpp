#include "pch.h"
#include "BECamera.h"
#include "BEMath.h"
#include "Submodules/imgui/imgui.h"

using namespace DirectX;

BECamera::BECamera() :
	position({0.0f, 0.0f, 0.0f, 0.0f}),
	direction({ 0.0f, 0.0f, -1.0f, 0.0f })
{
	Recalc();
}

BECamera::BECamera(XMVECTOR position, XMVECTOR direction) :
	position(position),
	direction(XMVector3Normalize(direction))
{
	Recalc();
}

inline void BECamera::Recalc()
{
	// axis
	right = XMVector3Normalize(XMVector3Cross(direction, {0,1,0,1}));
	up = XMVector3Normalize( XMVector3Cross(right, direction) );

	// ray info
	float tanFov = tanf(fieldOfViewY * 0.5f);
	rightScaled = right * tanFov * viewPortRatioY;
	upScaled = up * tanFov;
	centre = position + direction;

	// matricies
	viewMatrix = XMMatrixLookToRH(position, direction, {0,1,0,1});
	projectionMatrix = XMMatrixPerspectiveFovRH(fieldOfViewY, viewPortRatioY, viewNear, viewDistance);
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void BECamera::SetPosition(DirectX::XMVECTOR _position)
{
	position = _position;
}

void BECamera::LookAt(XMVECTOR target)
{
	direction = XMVector3Normalize(target - position);

	Recalc();
}

BECamera::Ray BECamera::RelativeScreenPositionToRay(float x, float y)
{
	Ray r;
	r.origin = centre + x * rightScaled + y * upScaled;
	r.direction = XMVector3Normalize(r.origin - position);

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

void BECamera::ShowImgui()
{
	static bool show = true;
	ImGui::Begin("Camera", &show);

	ImGui::DragFloat3("Position", position.m128_f32);
	ImGui::DragFloat("Yaw", &mYaw);
	ImGui::DragFloat("Pitch", &mPitch, -pitchBound, pitchBound);
	ImGui::DragFloat("Near plane ", &viewNear);
	ImGui::DragFloat("Far plane ", &viewDistance);

	ImGui::End();
}
