#pragma once
#include "BEEntity.h"


using namespace DirectX;

class BECamera : public BEEntity
{
public:
	struct Ray
	{
		XMVECTOR position;
		XMVECTOR direction;
	};

	float viewPortRatio = 3.0f / 4.0f;
	float viewPortX = 1.0f;
	float viewPortY = viewPortRatio;
	//XMVECTOR viewPort = { 1.0f, viewPortRatio, 1.0f, 1.0f };

	float focalLength = 1.0f; // is this really a focal length?
	float maxDistance = 10000.0f; // to do: need to think about this number.

	XMVECTOR up = {0.0f, 1.0f, 0.0f, 1.0f};

	BECamera(XMVECTOR _position, XMVECTOR _direction);
	inline void Recalc();

	void LookAt(XMVECTOR target);

	inline XMMATRIX GetViewProjectionMatrix() { return viewMatrix * projectionMatrix; }
	inline XMMATRIX GetViewMatrix() { return viewMatrix; }
	inline XMMATRIX GetProjectionMatrix() { return projectionMatrix; }

	inline XMVECTOR WorldToScreen(XMVECTOR coord) { return XMVector3TransformCoord(coord, viewMatrix * projectionMatrix); }

	Ray RelativeScreenPositionToRay(float x, float y);

	void Pan(float _right, float _up, float _forward);
	void RotateDirection(float yaw, float pitch, float roll);
	void RotatePosition(float yaw, float pitch);

	float DirectionDot(XMVECTOR v) { return XMVectorGetX(XMVector3Dot(direction, v)); };

	inline bool OveralpsScreen(XMVECTOR _v) { 
		float x = XMVectorGetX(_v);
		float y = XMVectorGetY(_v);
		return ((x >= -1.0f && x <= 1.0f) ||  // (x or
		        (y >= -1.0f && y <= 1.0f))    //  y in the screen)
			   && XMVectorGetZ(_v) > 0.0f; }  // and infront of screen

private:
	XMVECTOR direction = { 0.0f, 0.0f, 1.0f ,1.0f};
	XMVECTOR right = { 1.0f, 0.0f, 0.0f ,1.0f };
	XMVECTOR upScaled = { 0.0f, 1.0f, 0.0f ,1.0f };

	XMVECTOR centre;

	XMMATRIX projectionMatrix;
	XMMATRIX viewMatrix;

};

