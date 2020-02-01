#include "BELight.h"

using namespace DirectX;

XMVECTOR BELightDirectional::Calculate(XMVECTOR dir, XMVECTOR nor)
{
	XMVECTOR dot = XMVector3Dot(nor, dir);
	if (dot.m128_f32[0] >= 0.0f) return g_XMZero;
	return XMVectorNegate(dot) * color;
}

XMVECTOR BEPointLight::CalculateColorInWorldSpace(XMVECTOR targetPoint, XMVECTOR normal)
{
	XMVECTOR dirToLight = position - targetPoint;

	// get the angle with the normal. Zero if not in the same general direction
	XMVECTOR dot = XMVector3Dot(normal, XMVector3Normalize(dirToLight));

	if (dot.m128_f32[0] <= 0.0f) return g_XMZero;

	// get the distance. Zero if out of range
	float distance = XMVector3Length(dirToLight).m128_f32[0];

	if (distance > maxDistance) return g_XMZero;

	// get the light strength
	float attenuation = 1.0f + linear * distance + quadratic * distance * distance;
	float luminosity = 1.0f / attenuation;

	XMVECTOR light = luminosity * dot * color;

	return light;
}

DirectX::XMVECTOR BEPointLight::CalculateColorSpecInWorldSpace(DirectX::XMVECTOR targetPoint, DirectX::XMVECTOR normal, DirectX::XMVECTOR cameraPosition)
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
			light = luminosity * dot * color;
		}
	}

	// calc the specular light
	XMVECTOR toCamera = XMVector3Normalize(cameraPosition - targetPoint);
	XMVECTOR reflectedLight = XMVector3Normalize(2.0f * XMVector3Dot(normal, toLight) * normal - toLight);

	float specPower = 40.0f; // to do: temp values... use material values later
	float specIntensity = 3.0f;

	float spec = XMVectorSaturate(XMVector3Dot(reflectedLight, toCamera)).m128_f32[0];
	
	if (spec > 0.0f) // skip the below calcs if zero
	{
		spec = powf(spec, specPower) * specIntensity;
	}

	return spec * color + light;
}
