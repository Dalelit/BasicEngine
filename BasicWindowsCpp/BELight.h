#pragma once
#include "BEEntity.h"

class BEPointLight : public BEEntity
{
public:
	DirectX::XMVECTOR color = { 0.5,0.5,0.5,1 };
	float maxDistance = 100.0f;

	// picking values from here... http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	float linear = 0.045f;
	float quadratic = 0.0075f;

	DirectX::XMVECTOR CalculateColorInWorldSpace(DirectX::XMVECTOR targetPoint, DirectX::XMVECTOR normal);
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

	inline DirectX::XMVECTOR CalculateColorInWorldSpace(DirectX::XMVECTOR normal) {
		return Calculate(direction, normal);
	}

	inline void SetDirection(DirectX::XMVECTOR _direction) { direction = DirectX::XMVector3Normalize(_direction); };
	inline DirectX::XMVECTOR GetDireciton() { return direction; };

	DirectX::XMVECTOR Calculate(DirectX::XMVECTOR dir, DirectX::XMVECTOR nor);

private:
	DirectX::XMVECTOR direction = { 1,1,1,1 };
};
