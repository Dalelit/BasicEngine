#include "BEMesh.h"

BEMesh::BEMesh(unsigned int _vCount, unsigned int _tCount)
{
	if (_vCount == 0) return;

	vCount = _vCount;
	verticies = new Vector3[vCount];
	//colors = new Color[vCount];

	tCount = _tCount;
	tBufferSize = tCount * 3;
	if (tCount > 0) triangles = new unsigned int[tBufferSize];
}

BEMesh::~BEMesh()
{
	if (verticies) delete verticies;
	if (colors) delete colors;
	if (triangles) delete triangles;
	if (lines) delete lines;
}

void BEMesh::AddLines(unsigned int _lCount)
{
	lCount = _lCount;
	lBufferSize = lCount * 2;
	if (lCount > 0) lines = new unsigned int[lBufferSize];
}