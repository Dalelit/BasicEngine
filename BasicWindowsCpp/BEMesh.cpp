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
		normals = new Vector3[nCount];
	}

	unsigned int tindx = 0;
	unsigned int* pt = triangles;

	while (tindx < tCount)
	{
		Vector3 v0 = verticies[*(pt++)];
		Vector3 v1 = verticies[*(pt++)] - v0;
		Vector3 v2 = verticies[*(pt++)] - v0;

		normals[tindx] = (v1.Cross(v2));
		normals[tindx].Normalize();

		tindx++;
	}
}
