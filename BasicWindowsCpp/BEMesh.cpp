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
		indxCount = _triCount * 3;
		indicies = new unsigned int[indxCount];
	}
}

BEMesh::BEMesh(std::vector<XMFLOAT3> _verticies, std::vector<XMFLOAT3> _normals)
{
	// note: assumes 1 normal for a face (i.e. for 3 verticies).
	// to do: tidy this up and have it handle either 1 or 3 normals per vertex

	topology = BEMeshTopology::TRIANGLE_LIST;
	vertCount = (unsigned int)_verticies.size();

	if (vertCount == 0) return;

	verticies = new BEVertex[vertCount];
	BEVertex* pv = verticies;

	for (auto v : _verticies)
	{
		pv->position = XMLoadFloat3(&v);
		pv->position.m128_f32[3] = 1.0f;
		pv->color = defaultColor;
		pv++;
	}

	pv = verticies;
	for (auto n : _normals)
	{
		pv->normal = XMLoadFloat3(&n);
		pv->normal.m128_f32[3] = 1.0f;
		pv++;
		pv->normal = XMLoadFloat3(&n);
		pv->normal.m128_f32[3] = 1.0f;
		pv++;
		pv->normal = XMLoadFloat3(&n);
		pv->normal.m128_f32[3] = 1.0f;
		pv++;
	}

	indxCount = vertCount;
	indicies = new unsigned int[indxCount];

	for (unsigned int j = 0; j < indxCount; j++)
	{
		indicies[j] = j;
	}

	SetBounds();
}

BEMesh::BEMesh(std::vector<DirectX::XMFLOAT3> _verticies, std::vector<DirectX::XMFLOAT3> _normals, std::vector<DirectX::XMFLOAT2> _texcoord, std::vector<unsigned int> _index)
{
	// note: assumes 1 normal per vertex. Like the other constructure... need to tidy up.

	topology = BEMeshTopology::TRIANGLE_INDEX;
	vertCount = (unsigned int)_verticies.size();

	if (vertCount == 0) return;

	verticies = new BEVertex[vertCount];
	BEVertex* pv = verticies;

	for (auto v : _verticies)
	{
		pv->position = XMLoadFloat3(&v);
		pv->position.m128_f32[3] = 1.0f;
		pv->color = defaultColor;
		pv++;
	}

	pv = verticies;
	for (auto n : _normals)
	{
		pv->normal = XMVector3Normalize(XMLoadFloat3(&n));
		pv->normal.m128_f32[3] = 1.0f;
		pv++;
	}

	pv = verticies;
	for (auto n : _texcoord)
	{
		pv->texcoord = n;
	}

	indxCount = (unsigned int)_index.size();
	indicies = new unsigned int[indxCount];

	unsigned int* pi = indicies;
	for (auto n : _index)
	{
		*pi = n;
		pi++;
	}

	SetBounds();
}

BEMesh::~BEMesh()
{
	if (verticies) delete verticies;
	if (indicies) delete indicies;
}

void BEMesh::SwapIndexOrder()
{
	for (unsigned int i = 0; i < indxCount; i += 3)
	{
		std::swap(indicies[i + 1], indicies[i + 2]);
	}
}
