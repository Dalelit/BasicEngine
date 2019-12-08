#pragma once
#include "BEEntity.h"

using namespace DirectX;

class BEMeshPrimatives
{
public:
	static BEMesh* TriangleMesh();
	static BEMesh* CubeMesh();
	static BEMesh* Ground(float width, float depth, unsigned int segmentsWide, unsigned int segmentsDeep);
	//static BEMesh* AxisMesh();
};

