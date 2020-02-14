#pragma once
#include "BEEntity.h"

////////////////////
// to do
// - have different functions for texture sampled vs not.

class BEPointLight : public BEEntity
{
public:
	DirectX::XMVECTOR color = { 0.5,0.5,0.5,1 };
	float maxDistance = 100.0f;

	// picking values from here... http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	float linear = 0.045f;
	float quadratic = 0.0075f;

	DirectX::XMVECTOR CalculateColorSpecInWorldSpace(DirectX::XMVECTOR targetPoint, DirectX::XMVECTOR normal, DirectX::XMVECTOR cameraPosition, BEMaterial* pMaterial, DirectX::XMVECTOR diffColor);

	inline DirectX::XMVECTOR CalculateColorSpecInWorldSpace(DirectX::XMVECTOR targetPoint, DirectX::XMVECTOR normal, DirectX::XMVECTOR cameraPosition, BEMaterial* pMaterial) {
		return CalculateColorSpecInWorldSpace(targetPoint, normal, cameraPosition, pMaterial, pMaterial->diffuseColor);
	};
};

class BELightAmbient
{
public:
	DirectX::XMVECTOR color = { 0.2f, 0.2f, 0.2f, 1.0f };

	BELightAmbient() = default;
	BELightAmbient(DirectX::XMVECTOR color) : color(color) {};

	DirectX::XMVECTOR CalculateColor(BEMaterial* pMaterial);
};

class BELightDirectional
{
public:
	DirectX::XMVECTOR color = { 0.5,0.5,0.5,1 };

	BELightDirectional() = default;
	BELightDirectional(DirectX::XMVECTOR color) : color(color) {};

	DirectX::XMVECTOR CalculateColorInWorldSpace(DirectX::XMVECTOR normal, BEMaterial* pMaterial, DirectX::XMVECTOR diffColor);
	
	DirectX::XMVECTOR CalculateColorInWorldSpace(DirectX::XMVECTOR normal, BEMaterial* pMaterial) {
		return CalculateColorInWorldSpace(normal, pMaterial, pMaterial->diffuseColor);
	};

	inline void SetDirection(DirectX::XMVECTOR _direction) { direction = DirectX::XMVector3Normalize(_direction); };
	inline DirectX::XMVECTOR GetDireciton() { return direction; };

private:
	DirectX::XMVECTOR direction = { 1,1,1,1 };
};
