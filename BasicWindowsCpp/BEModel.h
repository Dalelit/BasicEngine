#pragma once
#include "BEEntity.h"

using namespace DirectX;
using namespace SimpleMath;

class BEModel
{
public:
	static BEMesh* TriangleMesh();
	static BEMesh* CubeMesh();


	// to do: delete

	//unsigned int vCount = 0;
	//unsigned int lCount = 0;
	//Vector3* verticies = NULL;
	//Vector3* screenPoints = NULL;
	//unsigned int* lines = NULL;

	//Color color = {1, 1, 1, 1};
	//Color* colors = NULL; // verticies color. If null assume single color

	//BEModel();
	//~BEModel();

	//void Translate(Vector3 v);

	//void CreateColorData();

	//static BEModel* AxisPrimative();
	//static BEModel* TrianglePrimative();
	//static BEModel* CubePrimative();

};

