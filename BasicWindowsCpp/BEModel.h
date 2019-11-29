#pragma once
#include "BEEntity.h"

using namespace DirectX;

class BEModel
{
public:
	static BEMesh* TriangleMesh();
	static BEMesh* CubeMesh();
	static BEMesh* AxisMesh();
};

