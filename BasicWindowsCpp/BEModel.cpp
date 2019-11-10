#include "BEModel.h"

BEMesh* BEModel::TriangleMesh()
{
	BEMesh* m = new BEMesh(3, 1 * 3);

	m->verticies[0] = { 0,0,0 };
	m->verticies[1] = { 1,0.5,0 };
	m->verticies[2] = { 0.5,1,0 };

	// clockwise
	// to do: check if clockwise is a good direction
	m->triangles[0] = 0;
	m->triangles[1] = 1;
	m->triangles[2] = 2;

	return m;
}

BEMesh* BEModel::CubeMesh()
{
	BEMesh* m = new BEMesh(8, 12 * 3);

	//    3--2
	//   /  /|
	//  0--1 6
	//  |  |/
	//  4--5


	m->verticies[0] = { -0.5f, 0.5f, -0.5f };
	m->verticies[1] = { 0.5f, 0.5f, -0.5f };
	m->verticies[2] = { 0.5f, 0.5f,  0.5f };
	m->verticies[3] = { -0.5f, 0.5f,  0.5f };
	m->verticies[4] = { -0.5f, -0.5f, -0.5f };
	m->verticies[5] = { 0.5f, -0.5f, -0.5f };
	m->verticies[6] = { 0.5f, -0.5f,  0.5f };
	m->verticies[7] = { -0.5f, -0.5f,  0.5f };
	
	int i = 0;

	// clockwise
	// to do: check if clockwise is a good direction
	m->triangles[i++] = 0; m->triangles[i++] = 1; m->triangles[i++] = 2; // top
	m->triangles[i++] = 0; m->triangles[i++] = 2; m->triangles[i++] = 3; // top
	m->triangles[i++] = 4; m->triangles[i++] = 5; m->triangles[i++] = 1; // front
	m->triangles[i++] = 4; m->triangles[i++] = 1; m->triangles[i++] = 0; // front
	m->triangles[i++] = 5; m->triangles[i++] = 6; m->triangles[i++] = 1; // right
	m->triangles[i++] = 1; m->triangles[i++] = 6; m->triangles[i++] = 2; // right
	m->triangles[i++] = 6; m->triangles[i++] = 3; m->triangles[i++] = 2; // back
	m->triangles[i++] = 6; m->triangles[i++] = 7; m->triangles[i++] = 3; // back
	m->triangles[i++] = 4; m->triangles[i++] = 0; m->triangles[i++] = 7; // left
	m->triangles[i++] = 0; m->triangles[i++] = 3; m->triangles[i++] = 7; // left
	m->triangles[i++] = 5; m->triangles[i++] = 4; m->triangles[i++] = 6; // bottom
	m->triangles[i++] = 4; m->triangles[i++] = 7; m->triangles[i++] = 6; // bottom

	return m;
}

// To do: delete

//BEModel::BEModel()
//{
//}
//
//BEModel::~BEModel()
//{
//	if (lCount > 0)
//	{
//		delete verticies;
//		delete screenPoints;
//		delete lines;
//	}
//}
//
//void BEModel::Translate(Vector3 v)
//{
//	unsigned int i = 0;
//
//	while (i < vCount)
//	{
//		verticies[i++] += v;
//	}
//}
//
//void BEModel::CreateColorData()
//{
//	if (colors != NULL) return; // already created
//
//	colors = new Color[vCount];
//
//	for (unsigned int i = 0; i < vCount; i++) colors[i] = color;
//}

///////////////////// primatives

//BEModel* BEModel::TrianglePrimative()
//{
//	BEModel* mp = new BEModel();
//
//	mp->vCount = 3;
//	mp->lCount = 2 * 3;
//	mp->verticies = new Vector3[mp->vCount];
//	mp->screenPoints = new Vector3[mp->vCount];
//	mp->lines = new unsigned int[mp->lCount];
//
//	int i = 0;
//	mp->verticies[i++] = { 0,0,0 };
//	mp->verticies[i++] = { 1,0,0 };
//	mp->verticies[i++] = { 0,1,0 };
//
//	i = 0;
//	mp->lines[i++] = 0; mp->lines[i++] = 1;
//	mp->lines[i++] = 1; mp->lines[i++] = 2;
//	mp->lines[i++] = 2; mp->lines[i++] = 0;
//
//	return mp;
//}
//
//BEModel* BEModel::AxisPrimative()
//{
//	BEModel* mp = new BEModel();
//
//	mp->vCount = 4;
//	mp->lCount = 2 * 3;
//	mp->verticies = new Vector3[mp->vCount];
//	mp->screenPoints = new Vector3[mp->vCount];
//	mp->colors = new Color[mp->vCount];
//	mp->lines = new unsigned int[mp->lCount];
//
//	int i = 0;
//	mp->verticies[i] = { 0,0,0 };
//	mp->colors[i++]  = { 1,1,1 };
//	mp->verticies[i] = { 1,0,0 };
//	mp->colors[i++]  = { 1,0,0 };
//	mp->verticies[i] = { 0,1,0 };
//	mp->colors[i++]  = { 0,1,0 };
//	mp->verticies[i] = { 0,0,1 };
//	mp->colors[i++]  = { 0,0,1 };
//
//	i = 0;
//	mp->lines[i++] = 0; mp->lines[i++] = 1;
//	mp->lines[i++] = 0; mp->lines[i++] = 2;
//	mp->lines[i++] = 0; mp->lines[i++] = 3;
//
//	return mp;
//}
//
//BEModel* BEModel::CubePrimative()
//{
//	BEModel* mp = new BEModel();
//
//	mp->vCount = 8;
//	mp->lCount = 2 * 12;
//	mp->verticies = new Vector3[mp->vCount];
//	mp->screenPoints = new Vector3[mp->vCount];
//	mp->lines = new unsigned int[mp->lCount];
//
//	int i = 0;
//	mp->verticies[i++] = { -0.5f, 0.5f, -0.5f };
//	mp->verticies[i++] = { 0.5f, 0.5f, -0.5f };
//	mp->verticies[i++] = { 0.5f, 0.5f,  0.5f };
//	mp->verticies[i++] = { -0.5f, 0.5f,  0.5f };
//	mp->verticies[i++] = { -0.5f, -0.5f, -0.5f };
//	mp->verticies[i++] = { 0.5f, -0.5f, -0.5f };
//	mp->verticies[i++] = { 0.5f, -0.5f,  0.5f };
//	mp->verticies[i++] = { -0.5f, -0.5f,  0.5f };
//
//	i = 0;
//	mp->lines[i++] = 0; mp->lines[i++] = 1;
//	mp->lines[i++] = 1; mp->lines[i++] = 2;
//	mp->lines[i++] = 2; mp->lines[i++] = 3;
//	mp->lines[i++] = 3; mp->lines[i++] = 0;
//	mp->lines[i++] = 0; mp->lines[i++] = 4;
//	mp->lines[i++] = 1; mp->lines[i++] = 5;
//	mp->lines[i++] = 2; mp->lines[i++] = 6;
//	mp->lines[i++] = 3; mp->lines[i++] = 7;
//	mp->lines[i++] = 4; mp->lines[i++] = 5;
//	mp->lines[i++] = 5; mp->lines[i++] = 6;
//	mp->lines[i++] = 6; mp->lines[i++] = 7;
//	mp->lines[i++] = 7; mp->lines[i++] = 4;
//
//	return mp;
//}

