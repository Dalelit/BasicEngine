#include "BEMesh.h"

BEMesh::BEMesh(unsigned int _vertCount, unsigned int _triCount, BEMeshTopology _topology)
{
	topology = _topology;

	if (_vertCount == 0) return;

	vertCount = _vertCount;
	verticies = new BEVertex[vertCount];

	if (_triCount > 0)
	{
		triCount = _triCount;
		triangles = new BETriangle[triCount];

		indxCount = triCount * 3;
		indicies = new unsigned int[indxCount];
	}
}

BEMesh::~BEMesh()
{
	if (verticies) delete verticies;
	if (indicies) delete indicies;
	if (triangles) delete triangles;
	if (lines) delete lines;
}

void BEMesh::AddLines(unsigned int _lCount)
{
	lCount = _lCount;
	lBufferSize = lCount * 2;
	if (lCount > 0) lines = new unsigned int[lBufferSize];
}

void BEMesh::CalculateTriangleInfo()
{
	// To Do: sort out if I need to skip this if it's a triangle list only.

	// sort out the indx info
	unsigned int* pIndx = indicies;
	for (unsigned int tindx = 0; tindx < triCount; tindx++)
	{
		triangles[tindx].indx = pIndx;
		pIndx += 3;
	}

	for (unsigned int tindx = 0; tindx < triCount; tindx++)
	{
		XMVECTOR v0 = verticies[triangles[tindx].indx[0]].position;
		XMVECTOR v1 = verticies[triangles[tindx].indx[1]].position - v0;
		XMVECTOR v2 = verticies[triangles[tindx].indx[2]].position - v0;
	}
}
