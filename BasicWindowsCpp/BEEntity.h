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
	BEEntity* pEntity = nullptr;
};

class BEEntity
{
public:
	bool active = true;
	BEEntity* parent = nullptr;
	std::vector<BEEntity*> children;

	DirectX::XMVECTOR scale = { 1,1,1,1 };
	DirectX::XMVECTOR position = { 0,0,0,1 };
	DirectX::XMVECTOR rotation = { 0,0,0,1 };
	
	BEMaterial material; // to do: work out where this should be

	BEEntity() = default;
	BEEntity(DirectX::XMFLOAT3A _position);

	~BEEntity();

	std::vector<BEEntityComponent*> components;

	// Note: update sets the internal matricies once. They don't happen in the various other functions.
	void Update(float deltaTime);

	inline void SetScale(DirectX::XMVECTOR _scale) { scale = _scale; }
	inline void SetScale(float x, float y, float z) { scale = { x, y, z, 1.0f }; }

	inline void SetPosition(DirectX::XMVECTOR _position) { position = _position; }
	inline void SetPosition(float x, float y, float z) { position = { x, y, z, 1.0f }; }

	inline void SetRotation(DirectX::XMVECTOR _rotation) { rotation = _rotation; }
	inline void SetRotation(float pitch, float yaw, float roll) { rotation = { pitch, yaw, roll, 1.0f }; }

	inline void Translate(DirectX::XMVECTOR vector) { position = DirectX::XMVectorAdd(position, vector); }
	inline void Translate(float x, float y, float z) { Translate({ x, y, z, 0.0f }); }

	inline void Rotate(float pitch, float yaw, float roll) { rotation = DirectX::XMVectorAdd(rotation, { pitch, yaw, roll, 0.0f }); }

	inline DirectX::XMVECTOR GetWorldPosition() { return DirectX::XMVector3TransformCoord(position, GetTransform()); }

	inline DirectX::XMMATRIX GetTransform() { return transform; }
	inline DirectX::XMMATRIX GetTransformRotation() { return transformRotation; }

	// note: assuming this isn't called much so not precalculating it.
	inline DirectX::XMMATRIX GetTransformInverse() { return DirectX::XMMatrixInverse(nullptr, transform); }
	inline DirectX::XMMATRIX GetTransformRotationInverse() { return DirectX::XMMatrixInverse(nullptr, transformRotation); }

	inline DirectX::XMVECTOR ModelToWorldPosition(DirectX::XMVECTOR point) { return DirectX::XMVector4Transform(point, transform); }
	inline DirectX::XMVECTOR ModelToWorldDirection(DirectX::XMVECTOR direction) { return DirectX::XMVector4Transform(direction, transformRotation); } // to do: normialise?

	inline DirectX::XMVECTOR WorldToModelPosition(DirectX::XMVECTOR point) { return DirectX::XMVector4Transform(point, GetTransformInverse()); }
	inline DirectX::XMVECTOR WorldToModelDirection(DirectX::XMVECTOR direction) { return DirectX::XMVector4Transform(direction, GetTransformRotationInverse()); } // to do: normialise?

	inline float GetDistanceInWorldSpace(DirectX::XMVECTOR worldPosition) { return DirectX::XMVector3Length(DirectX::XMVectorSubtract(worldPosition, GetWorldPosition())).m128_f32[0]; }

protected:
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX transformRotation = DirectX::XMMatrixIdentity();
};

