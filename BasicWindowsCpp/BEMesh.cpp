#include "BEMesh.h"

BEMesh::BEMesh(unsigned int _vCount, unsigned int _tCount)
{
	if (_vCount == 0) return;

	vCount = _vCount;
	verticies = new XMVECTOR[vCount];
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
	if (normals) delete normals;
	if (lines) delete lines;
}

void BEMesh::AddLines(unsigned int _lCount)
{
	lCount = _lCount;
	lBufferSize = lCount * 2;
	if (lCount > 0) lines = new unsigned int[lBufferSize];
}

void BEMesh::CalculateNormals()
{
	// to do: create a version where there is 1 normal per triangle instead?
	if (!normals)
	{
		nCount = tCount;
		normals = new XMVECTOR[nCount];
	}

	unsigned int tindx = 0;
	unsigned int* pt = triangles;

	while (tindx < tCount)
	{
		XMVECTOR v0 = verticies[*(pt++)];
		XMVECTOR v1 = verticies[*(pt++)] - v0;
		XMVECTOR v2 = verticies[*(pt++)] - v0;

		normals[tindx] = XMVector3Cross(v1, v2);
		XMVector3Normalize(normals[tindx]);

		tindx++;
	}
}
