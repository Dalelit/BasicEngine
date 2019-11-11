#pragma once
#include "BEEntity.h"

using namespace DirectX;
using namespace SimpleMath;

class BEModel
{
public:
	static BEMesh* TriangleMesh();
	static BEMesh* CubeMesh();
	static BEMesh* AxisMesh();
};

