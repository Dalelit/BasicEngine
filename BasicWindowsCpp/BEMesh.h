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
	unsigned int tBufferSize = 0;
	unsigned int* triangles = NULL;

	unsigned int lCount = 0;
	unsigned int lBufferSize = 0;
	unsigned int* lines = NULL;

	// vCount = number of verticies, tCount = number of triangles. Reminder - Internal buffer for triangles is x3 size.
	BEMesh(unsigned int _vCount, unsigned int _tCount);
	~BEMesh();

	void AddLines(unsigned int _lCount);

	inline void Translate(Vector3 v) { for (unsigned int i = 0; i < vCount; i++) verticies[i] += v; }
	inline void Scale(float f) { for (unsigned int i = 0; i < vCount; i++) verticies[i] *= f; }
	inline void Scale(Vector3 v) { for (unsigned int i = 0; i < vCount; i++) verticies[i] *= v; }
};

