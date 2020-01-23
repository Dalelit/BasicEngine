#pragma once
#include <DirectXMath.h>

class BECamera
{
public:
	struct Ray
	{
		DirectX::XMVECTOR origin;
		DirectX::XMVECTOR direction;
	};

	DirectX::XMVECTOR position = { 0,0,0,1 };
	
	float viewPortRatio = 3.0f / 4.0f;
	float viewPortX = 1.0f;
	float viewPortY = viewPortRatio;
	//DirectX::XMVECTOR viewPort = { 1.0f, viewPortRatio, 1.0f, 1.0f };

	float viewNear = 1.0f; // is this really a focal length?
	float viewDistance = 10000.0f; // to do: need to think about this number.

	DirectX::XMVECTOR up = {0.0f, 1.0f, 0.0f, 1.0f};

	BECamera(DirectX::XMVECTOR _position, DirectX::XMVECTOR _direction);
	inline void Recalc();

	inline void SetPosition(DirectX::XMVECTOR _position) { position = _position; }
	inline void SetPosition(float x, float y, float z) { position = { x, y, z, 1.0f }; }

	void LookAt(DirectX::XMVECTOR target);
	inline void LookAt(float x, float y, float z) { LookAt({ x, y, z, 1.0f }); };

	inline DirectX::XMMATRIX GetViewProjectionMatrix() { return viewProjectionMatrix; }
	inline DirectX::XMMATRIX GetViewMatrix() { return viewMatrix; }
	inline DirectX::XMMATRIX GetProjectionMatrix() { return projectionMatrix; }

	inline DirectX::XMVECTOR WorldToScreen(DirectX::XMVECTOR coord) { return DirectX::XMVector3TransformCoord(coord, viewProjectionMatrix); }

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
	DirectX::XMVECTOR direction = { 0.0f, 0.0f, 1.0f ,1.0f};
	DirectX::XMVECTOR right = { 1.0f, 0.0f, 0.0f ,1.0f };
	DirectX::XMVECTOR upScaled = { 0.0f, 1.0f, 0.0f ,1.0f };

	float mYaw = 0.0f;
	float mPitch = 0.0f;

	DirectX::XMVECTOR centre;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX viewProjectionMatrix;

};

