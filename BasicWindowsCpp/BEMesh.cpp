#include "BEMesh.h"

BEMesh::BEMesh(unsigned int _vCount, unsigned int _tCount)
{
	assert(_vCount > 0 && _tCount > 0);

	vCount = _vCount;
	verticies = new Vector3[vCount];
	colors = new Color[vCount];

	tCount = _tCount;
	triangles = new unsigned int[tCount];
}

BEMesh::~BEMesh()
{
	delete verticies;
	delete colors;
	delete triangles;
}
