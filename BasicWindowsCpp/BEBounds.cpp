#include "pch.h"
#include "BEBounds.h"

using namespace DirectX;

void BEBoundsBox::SetBounds(unsigned int vertexCount, BEVertex* verticies)
{
	XMVECTOR min = g_FltMax;
	XMVECTOR max = g_FltMin;

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		XMVECTOR v = verticies->position;
		max = XMVectorMax(max, v);
		min = XMVectorMin(min, v);
		verticies++;
	}

	BoundingBox::CreateFromPoints(box, min, max);
}

void BEBoundsSphere::SetBounds(unsigned int vertexCount, BEVertex* verticies)
{
	std::vector<XMFLOAT3> points;
	points.reserve(vertexCount);

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, verticies->position);
		points.emplace_back(pos);
		verticies++;
	}

	BoundingSphere::CreateFromPoints(sphere, points.size(), points.data(), sizeof(XMFLOAT3));
}
