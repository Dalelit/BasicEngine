#pragma once
#include "BEEntity.h"

class BECamera : public BEEntity
{
public:
	struct Ray
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR direction;
	};

	float viewPortRatio = 3.0f / 4.0f;
	float viewPortX = 1.0f;
	float viewPortY = viewPortRatio;
	//DirectX::XMVECTOR viewPort = { 1.0f, viewPortRatio, 1.0f, 1.0f };

	float focalLength = 1.0f; // is this really a focal length?
	float maxDistance = 10000.0f; // to do: need to think about this number.

	DirectX::XMVECTOR up = {0.0f, 1.0f, 0.0f, 1.0f};

	BECamera(DirectX::XMVECTOR _position, DirectX::XMVECTOR _direction);
	inline void Recalc();

	void LookAt(DirectX::XMVECTOR target);

	inline DirectX::XMMATRIX GetViewProjectionMatrix() { return viewMatrix * projectionMatrix; }
	inline DirectX::XMMATRIX GetViewMatrix() { return viewMatrix; }
	inline DirectX::XMMATRIX GetProjectionMatrix() { return projectionMatrix; }

	inline DirectX::XMVECTOR WorldToScreen(DirectX::XMVECTOR coord) { return DirectX::XMVector3TransformCoord(coord, viewMatrix * projectionMatrix); }

	inline bool IsVisible(DirectX::XMVECTOR point, DirectX::XMVECTOR normal) { return DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(point, position), normal)) < 0.0f; }

	Ray RelativeScreenPositionToRay(float x, float y);

	void Pan(float _right, float _up, float _forward);
	void RotateDirection(float yaw, float pitch, float roll);
	void RotatePosition(float yaw, float pitch);

	float DirectionDot(DirectX::XMVECTOR v) { return DirectX::XMVectorGetX(DirectX::XMVector3Dot(direction, v)); };

	inline bool OveralpsScreen(DirectX::XMVECTOR _v) { 
		float x = DirectX::XMVectorGetX(_v);
		float y = DirectX::XMVectorGetY(_v);
		return ((x >= -1.0f && x <= 1.0f) ||  // (x or
		        (y >= -1.0f && y <= 1.0f))    //  y in the screen)
			   && DirectX::XMVectorGetZ(_v) > 0.0f; }  // and infront of screen

private:
	DirectX::XMVECTOR direction = { 0.0f, 0.0f, 1.0f ,1.0f};
	DirectX::XMVECTOR right = { 1.0f, 0.0f, 0.0f ,1.0f };
	DirectX::XMVECTOR upScaled = { 0.0f, 1.0f, 0.0f ,1.0f };

	DirectX::XMVECTOR centre;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX viewMatrix;

};

