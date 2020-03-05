#pragma once
#include "pch.h"

class BECamera
{
public:
	struct Ray
	{
		DirectX::XMVECTOR origin;
		DirectX::XMVECTOR direction;
	};

	DirectX::XMVECTOR position = { 0,0,0,1 };
	
	float GetViewPortAspectRatio() { return viewPortRatioY; }
	void SetViewPortAspectRatio(float ratio) { viewPortRatioY = ratio; Recalc(); }

	float viewNear = 1.0f;
	float viewDistance = 1000.0f;

	BECamera();
	BECamera(DirectX::XMVECTOR position, DirectX::XMVECTOR direction);
	inline void Recalc();

	void SetPosition(DirectX::XMVECTOR _position);
	inline void SetPosition(float x, float y, float z) { SetPosition({ x, y, z, 1.0f }); }

	void LookAt(DirectX::XMVECTOR target);
	inline void LookAt(float x, float y, float z) { LookAt({ x, y, z, 1.0f }); };

	inline DirectX::XMMATRIX GetViewProjectionMatrix() { return viewProjectionMatrix; }
	inline DirectX::XMMATRIX GetViewMatrix() { return viewMatrix; }
	inline DirectX::XMMATRIX GetProjectionMatrix() { return projectionMatrix; }

	inline DirectX::XMVECTOR WorldToScreen(DirectX::XMVECTOR coord) { return DirectX::XMVector4Transform(coord, viewProjectionMatrix); }

	inline bool IsVisible(DirectX::XMVECTOR point, DirectX::XMVECTOR normal) { return DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(point, position), normal)) < 0.0f; }

	Ray RelativeScreenPositionToRay(float x, float y);

	void Pan(float _right, float _up, float _forward);
	void RotateDirection(float yaw, float pitch, float roll = 0.0f);
	void RotatePosition(float yaw, float pitch);

	float DirectionDot(DirectX::XMVECTOR v) { return DirectX::XMVectorGetX(DirectX::XMVector3Dot(direction, v)); };

	inline bool OveralpsScreen(DirectX::XMVECTOR _v) { 
		float x = DirectX::XMVectorGetX(_v);
		float y = DirectX::XMVectorGetY(_v);
		return ((x >= -1.0f && x <= 1.0f) &&  // (x and
		        (y >= -1.0f && y <= 1.0f))    //  y in the screen)
			   && DirectX::XMVectorGetZ(_v) > 0.0f; }  // and infront of screen


private:
	float viewPortRatioY = 4.0f / 3.0f;
	float fieldOfViewY = DirectX::XMConvertToRadians(74.0f);

	DirectX::XMVECTOR up = { 0.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR direction = { 0.0f, 0.0f, 1.0f ,1.0f};
	DirectX::XMVECTOR right = { 1.0f, 0.0f, 0.0f ,1.0f };

	// for creating rays
	DirectX::XMVECTOR centre;
	DirectX::XMVECTOR upScaled = { 0.0f, 1.0f, 0.0f ,1.0f };
	DirectX::XMVECTOR rightScaled = { 0.0f, 1.0f, 0.0f ,1.0f };

	float mYaw = 0.0f;
	float mPitch = 0.0f;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX viewProjectionMatrix;
};

