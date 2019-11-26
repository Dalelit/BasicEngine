#pragma once
//#include <d3d11.h>

// To do : check the why to do these includes
#include <DirectXMath.h>
//#include <DirectXMathVector.inl>
//#include <DirectXMathMatrix.inl>
//#include <DirectXMathConvert.inl>
//#include <DirectXMathMisc.inl>

//#include "SimpleMath.h"
//#include "SimpleMath.inl"

#define SWAPINT(A,B)   {int temp = A; A = B; B = temp; }
#define SWAPFLOAT(A,B) {float temp = A; A = B; B = temp; }
//#define SWAPVECTOR3(A,B) {Vector3 temp = A; A = B; B = temp; }
//#define SWAPCOLOR(A,B) {Color temp = A; A = B; B = temp; }
#define SWAPXMVECTOR(A,B) {XMVECTOR temp = A; A = B; B = temp; }
#define SWAPXMFLOAT3(A,B) {XMFLOAT3 temp = A; A = B; B = temp; }

// To do: check if there is a DirectX implementation for this stuff
// some XMFLOAT3 helpter functions...

//XMFLOAT3 operator+ (const XMFLOAT3& v1, const XMFLOAT3& v2) {
//	XMFLOAT3 r;
//	r.x = v1.x + v2.x;
//	r.y = v1.y + v2.y;
//	r.z = v1.z + v2.z;
//	return r;
//}

//XMFLOAT3 operator *(XMFLOAT3 l, XMFLOAT3 r) {
//	XMVECTOR lvec(XMLoadFloat3(&l));
//	XMVECTOR rvec(XMLoadFloat3(&r));
//	//Perform operations
//}

//XMFLOAT3 operator+ (const XMFLOAT3& v1, const XMFLOAT3& v2) { return XMFLOAT3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); };
//XMFLOAT3 operator- (const XMFLOAT3& v1, const XMFLOAT3& v2) { return { v1.x - v2.x, v1.y - v2.y , v1.z - v2.z }; };
//XMFLOAT3 operator* (const XMFLOAT3& v1, const XMFLOAT3& v2) { return { v1.x * v2.x, v1.y * v2.y , v1.z * v2.z }; };
//XMFLOAT3 operator/ (const XMFLOAT3& v1, const XMFLOAT3& v2) { return { v1.x / v2.x, v1.y / v2.y , v1.z / v2.z }; };
//XMFLOAT3 operator* (const XMFLOAT3& v1, float s) { return { v1.x * s, v1.y * s , v1.z * s }; };
//XMFLOAT3 operator* (float s, const XMFLOAT3& v1) { return { v1.x * s, v1.y * s , v1.z * s }; };
//XMFLOAT3 operator/ (const XMFLOAT3& v1, float s) { return { v1.x / s, v1.y / s , v1.z / s }; };
//XMFLOAT3 operator- (const XMFLOAT3& v1) { return { -v1.x, -v1.y, -v1.z }; };
