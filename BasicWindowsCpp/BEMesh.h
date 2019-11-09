#pragma once
#include "BECommon.h"

using namespace DirectX;
using namespace SimpleMath;

class BEMesh
{
public:
	unsigned int vCount = 0;
	Vector3* verticies = NULL;
	Color* colors = NULL;

	unsigned int tCount = 0;
	unsigned int* triangles = NULL;

	BEMesh(unsigned int _vCount, unsigned int _tCount);
	~BEMesh();

	inline void Translate(Vector3 v) { for (unsigned int i = 0; i < vCount; i++) verticies[i] += v; }
	inline void Scale(Vector3 v) { for (unsigned int i = 0; i < vCount; i++) verticies[i] *= v; }
};

