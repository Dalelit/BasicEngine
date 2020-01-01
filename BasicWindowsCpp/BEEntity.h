#pragma once
#include "BEMaterial.h"
#include <vector>
#include <math.h>

class BEEntity;

class BEEntityComponent
{
public:
	BEEntityComponent(BEEntity* pEntity);

	virtual void Update(float deltatime) = 0;
	BEEntity* parent = nullptr;
};

class BEEntity
{
public:
	bool active = true;
	BEEntity* parent = nullptr;
	std::vector<BEEntity*> children;

	DirectX::XMVECTOR position = { 0,0,0,1 };
	DirectX::XMVECTOR rotation = {};
	
	BEMaterial material; // to do: work out where this should be

	BEEntity() = default;
	BEEntity(DirectX::XMFLOAT3A _position);

	~BEEntity();

	std::vector<BEEntityComponent*> components;

	void Update(float deltaTime);

	void Translate(float x, float y, float z) {
		position.m128_f32[0] += x;
		position.m128_f32[1] += y;
		position.m128_f32[2] += z;
	}

	void Rotate(float pitch, float yaw, float roll) {
		rotation.m128_f32[0] += pitch;
		rotation.m128_f32[1] += yaw;
		rotation.m128_f32[2] += roll;
	}

	inline DirectX::XMMATRIX GetTransform() { return transform; };
	inline DirectX::XMMATRIX GetTransformRotation() { return transformRotation; };

protected:
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX transformRotation = DirectX::XMMatrixIdentity();
};

