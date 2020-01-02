#pragma once
#include "BEEntity.h"

class BEPointLight : public BEEntity
{
public:
	DirectX::XMVECTOR color = { 0.5,0.5,0.5,1 };

	DirectX::XMVECTOR CalculateColorInWorldSpace(DirectX::XMVECTOR point, DirectX::XMVECTOR normal);
};

class BELightAmbient
{
public:
	DirectX::XMVECTOR color = { 0.1f,0.1f,0.1f,1.0f };

	BELightAmbient() = default;
	BELightAmbient(DirectX::XMVECTOR color) : color(color) {};
};

class BELightDirectional
{
public:
	DirectX::XMVECTOR color = { 0.5,0.5,0.5,1 };

	BELightDirectional() = default;
	BELightDirectional(DirectX::XMVECTOR color) : color(color) {};

	//DirectX::XMVECTOR CalculateColorInModelSpace(DirectX::XMVECTOR normal);
	inline DirectX::XMVECTOR CalculateColorInWorldSpace(DirectX::XMVECTOR normal) {
		return Calculate(direction, normal);
	}

	inline void SetDirection(DirectX::XMVECTOR _direction) { direction = DirectX::XMVector3Normalize(_direction); };
	inline DirectX::XMVECTOR GetDireciton() { return direction; };

	DirectX::XMVECTOR Calculate(DirectX::XMVECTOR dir, DirectX::XMVECTOR nor);

private:
	DirectX::XMVECTOR direction = { 1,1,1,1 };
};
