#include "pch.h"
#include "BELight.h"

using namespace DirectX;

XMVECTOR BELightDirectional::CalculateColorInWorldSpace(DirectX::XMVECTOR normal, BEMaterial* pMaterial, DirectX::XMVECTOR diffColor)
{
	XMVECTOR dot = XMVector3Dot(normal, direction);
	if (dot.m128_f32[0] >= 0.0f) return g_XMZero;
	return XMVectorNegate(dot) * color * diffColor;
}

DirectX::XMVECTOR BEPointLight::CalculateColorSpecInWorldSpace(DirectX::XMVECTOR targetPoint, DirectX::XMVECTOR normal, DirectX::XMVECTOR cameraPosition, BEMaterial* pMaterial, DirectX::XMVECTOR diffColor)
{
	XMVECTOR toLight = position - targetPoint;

	// calc light from point source
	XMVECTOR light = g_XMZero;

	XMVECTOR dot = XMVector3Dot(normal, XMVector3Normalize(toLight)); // get the angle with the normal. Zero if not in the same general direction

	if (dot.m128_f32[0] > 0.0f)
	{
		float distance = XMVector3Length(toLight).m128_f32[0]; // get the distance. Zero if out of range

		if (distance <= maxDistance)
		{
			// get the light strength
			float attenuation = 1.0f + linear * distance + quadratic * distance * distance;
			float luminosity = 1.0f / attenuation;
			light = luminosity * dot * color * diffColor;
		}
	}

	// calc the specular light
	XMVECTOR toCamera = XMVector3Normalize(cameraPosition - targetPoint);
	XMVECTOR reflectedLight = XMVector3Normalize(2.0f * XMVector3Dot(normal, toLight) * normal - toLight);

	float specPower = pMaterial->specularExponent;
	//float specIntensity = 1.0f;

	float spec = XMVectorSaturate(XMVector3Dot(reflectedLight, toCamera)).m128_f32[0];
	
	if (spec > 0.0f) // skip the below calcs if zero
	{
		spec = powf(spec, specPower); // *specIntensity;
	}

	return spec * color * pMaterial->specularColor + light;
}

DirectX::XMVECTOR BELightAmbient::CalculateColor(BEMaterial* pMaterial)
{
	return color * pMaterial->ambientColor;
}
