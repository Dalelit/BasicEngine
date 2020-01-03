#pragma once
#include "BEVertex.h"
#include <DirectXCollision.h>

// Class for bounds, and also one to use if you don't want any processing time.
// To do: may need to change this to inheritence at some point rather than just the same signature.
//
class BEBounds
{
public:
	bool Intersects(DirectX::XMVECTOR rayPosition, DirectX::XMVECTOR rayDireciton) { return true; }
	void SetBounds(unsigned int vertexCount, BEVertex* verticies) {};
};

class BEBoundsBox
{
public:
	inline bool Intersects(DirectX::XMVECTOR rayPosition, DirectX::XMVECTOR rayDireciton) {
		float dist = 0.0f;
		return box.Intersects(rayPosition, rayDireciton, dist);
	}

	inline bool Intersects(DirectX::XMVECTOR rayPosition, DirectX::XMVECTOR rayDireciton, float maxDistance) {
		float dist = 0.0f;
		return box.Intersects(rayPosition, rayDireciton, dist) && dist < maxDistance;
	}

	void SetBounds(unsigned int vertexCount, BEVertex* verticies);

	DirectX::BoundingBox box;
};

class BEBoundsSphere
{
public:
	inline bool Intersects(DirectX::XMVECTOR rayPosition, DirectX::XMVECTOR rayDireciton) {
		float dist = 0.0f;
		return sphere.Intersects(rayPosition, rayDireciton, dist);
	}

	void SetBounds(unsigned int vertexCount, BEVertex* verticies);

	DirectX::BoundingSphere sphere;
};

