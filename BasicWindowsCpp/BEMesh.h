#pragma once
#include "BECommon.h"

using namespace DirectX;
//using namespace SimpleMath;

// Notes/Reminders/To check
// - RH coord system
// - traingle ordered in clockwise direction

class BEMesh
{
public:
	unsigned int vCount = 0;
	XMVECTOR* verticies = NULL;
	XMVECTOR* colors = NULL;

	unsigned int nCount = 0;
	XMVECTOR* normals = NULL;

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

	void CalculateNormals();

	inline void Translate(XMVECTOR v) { for (unsigned int i = 0; i < vCount; i++) verticies[i] += v; }
	inline void Scale(float f) { for (unsigned int i = 0; i < vCount; i++) verticies[i] *= f; }
	inline void Scale(XMVECTOR v) { for (unsigned int i = 0; i < vCount; i++) verticies[i] *= v; }

	inline void Transform(XMMATRIX m) { for (unsigned int i = 0; i < vCount; i++) verticies[i] = XMVector3Transform(verticies[i], m); }
};

