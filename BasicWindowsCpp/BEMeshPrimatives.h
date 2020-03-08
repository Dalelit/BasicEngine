#pragma once
#include "pch.h"
#include "BEEntity.h"
#include "BEMesh.h"

class BEMeshPrimatives
{
public:
	static BEMesh* TriangleMesh();

	static BEMesh* CubeMesh();

	static BEMesh* Ground(
		float width, float depth,
		unsigned int segmentsWide, unsigned int segmentsDeep,
		float heightMin = 0.0f, float heightMax = 0.0f);

	static BEMesh* Icosphere(unsigned int iterations);

	// Finds if existing, or appends if not, a vertex returing the index
	static unsigned int AddUniqueVertex(std::vector<DirectX::XMVECTOR>& verticies, DirectX::XMVECTOR newVertex);
};

