#pragma once
#include <d3d12.h>
#include "SimpleMath.h"
#include "SimpleMath.inl"

#define SWAPINT(A,B)   {int temp = A; A = B; B = temp; }
#define SWAPFLOAT(A,B) {float temp = A; A = B; B = temp; }
#define SWAPVECTOR3(A,B) {Vector3 temp = A; A = B; B = temp; }
#define SWAPCOLOR(A,B) {Color temp = A; A = B; B = temp; }
