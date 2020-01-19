#pragma once

// To do : check the why to do these includes
#include <DirectXMath.h>

// taken from the directXMath library for XMVECTOR
inline DirectX::XMFLOAT2 BEXMFloat2BaryCentric(DirectX::XMFLOAT2 Position0, DirectX::XMFLOAT2 Position1, DirectX::XMFLOAT2 Position2, float f, float g)
{
	DirectX::XMFLOAT2 result;
	result.x = Position0.x + f * (Position1.x - Position0.x) + g * (Position2.x - Position0.x);
	result.y = Position0.y + f * (Position1.y - Position0.y) + g * (Position2.y - Position0.y);
	return result;
}


template<typename T>
T WrapAngle(T angle)
{
	float mod = fmodf((float)angle, DirectX::XM_2PI);

	if (mod < 0.0f) mod += DirectX::XM_2PI;

	return (T)mod;
}
