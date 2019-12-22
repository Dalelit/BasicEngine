#pragma once
//#include <d3d11.h>

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
