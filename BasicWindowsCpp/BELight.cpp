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
