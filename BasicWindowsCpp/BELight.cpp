#include "BELight.h"

using namespace DirectX;

XMVECTOR BELightDirectional::Calculate(XMVECTOR dir, XMVECTOR nor)
{
	XMVECTOR dot = XMVector3Dot(nor, dir);
	if (dot.m128_f32[0] >= 0.0f) return g_XMZero;
	return XMVectorNegate(dot) * color;
}

XMVECTOR BEPointLight::CalculateColorInWorldSpace(XMVECTOR point, XMVECTOR normal)
{
	return DirectX::XMVECTOR();
}
