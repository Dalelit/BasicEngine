#pragma once

// To do : check the why to do these includes
#include <DirectXMath.h>

// To do: use the standard library to do this.
#define SWAPINT(A,B)   {int temp = A; A = B; B = temp; }
#define SWAPFLOAT(A,B) {float temp = A; A = B; B = temp; }
//#define SWAPVECTOR3(A,B) {Vector3 temp = A; A = B; B = temp; }
//#define SWAPCOLOR(A,B) {Color temp = A; A = B; B = temp; }
#define SWAPXMVECTOR(A,B) {XMVECTOR temp = A; A = B; B = temp; }
#define SWAPXMFLOAT3(A,B) {XMFLOAT3 temp = A; A = B; B = temp; }

#define BE_HR_CHECK(hr) if (FAILED(hr)) throw hr;


// taken from the directXMath library for XMVECTOR
inline DirectX::XMFLOAT2 BEXMFloat2BaryCentric(DirectX::XMFLOAT2 Position0, DirectX::XMFLOAT2 Position1, DirectX::XMFLOAT2 Position2, float f, float g)
{
	DirectX::XMFLOAT2 result;
	result.x = Position0.x + f * (Position1.x - Position0.x) + g * (Position2.x - Position0.x);
	result.y = Position0.y + f * (Position1.y - Position0.y) + g * (Position2.y - Position0.y);
	return result;
}