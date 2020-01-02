#include "BEMesh.h"
#include <string>
#include <iostream>

using namespace DirectX;

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

BEMesh::BEMesh(std::vector<XMFLOAT3> _verticies, std::vector<XMFLOAT3> _normals, BEMeshTopology _topology)
{
	topology = _topology;
	vertCount = (unsigned int)_verticies.size();
	triCount = (unsigned int)_normals.size();

	if (vertCount == 0) return;

	verticies = new BEVertex[vertCount];
	unsigned int i = 0;

	for (auto v : _verticies)
	{
		XMVECTOR position = XMLoadFloat3(&v);
		position.m128_f32[3] = 1.0f;
		verticies[i].position = position;
		verticies[i].color = defaultColor;
		i++;
	}

	// to do: temp hack to check
	if (vertCount != _normals.size() * 3) throw "Temp hack: BEMesh expects normals to be 1/3 of verticies";

	i = 0;
	for (auto n : _normals)
	{
		XMVECTOR normal = XMLoadFloat3(&n);
		normal.m128_f32[3] = 1.0f;

		verticies[i++].normal = normal;
		verticies[i++].normal = normal;
		verticies[i++].normal = normal;
	}


	if (triCount > 0)
	{
		triangles = new BETriangle[triCount];

		indxCount = triCount * 3;
		indicies = new unsigned int[indxCount];

		for (unsigned int j = 0; j < indxCount; j++)
		{
			indicies[j] = j;
		}
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

	SetBounds();

	//for (unsigned int tindx = 0; tindx < triCount; tindx++)
	//{
	//	XMVECTOR v0 = verticies[triangles[tindx].indx[0]].position;
	//	XMVECTOR v1 = verticies[triangles[tindx].indx[1]].position - v0;
	//	XMVECTOR v2 = verticies[triangles[tindx].indx[2]].position - v0;
	//}
}
