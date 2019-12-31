#pragma once
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

};

