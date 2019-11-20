#include "BEModel.h"

BEMesh* BEModel::TriangleMesh()
{
	BEMesh* m = new BEMesh(3, 1);

	m->verticies[0] = { 0,0,0 };
	m->verticies[1] = { 1,0,0 };
	m->verticies[2] = { 0,1,0 };

	// clockwise
	// to do: check if clockwise is a good direction
	m->triangles[0] = 0;
	m->triangles[1] = 1;
	m->triangles[2] = 2;

	m->CalculateNormals();

	return m;
}

BEMesh* BEModel::CubeMesh()
{
	BEMesh* m = new BEMesh(8, 12);

	//    3--2
	//   /  /|
	//  0--1 6
	//  |  |/
	//  4--5


	m->verticies[0] = { -0.5f, 0.5f, 0.5f };
	m->verticies[1] = { 0.5f, 0.5f, 0.5f };
	m->verticies[2] = { 0.5f, 0.5f,  -0.5f };
	m->verticies[3] = { -0.5f, 0.5f,  -0.5f };
	m->verticies[4] = { -0.5f, -0.5f, 0.5f };
	m->verticies[5] = { 0.5f, -0.5f, 0.5f };
	m->verticies[6] = { 0.5f, -0.5f,  -0.5f };
	m->verticies[7] = { -0.5f, -0.5f,  -0.5f };
	
	int i = 0;

	// clockwise
	// to do: check if clockwise is a good direction
	m->triangles[i++] = 0; m->triangles[i++] = 1; m->triangles[i++] = 2; // top
	m->triangles[i++] = 0; m->triangles[i++] = 2; m->triangles[i++] = 3; // top
	m->triangles[i++] = 4; m->triangles[i++] = 5; m->triangles[i++] = 1; // front
	m->triangles[i++] = 4; m->triangles[i++] = 1; m->triangles[i++] = 0; // front
	m->triangles[i++] = 1; m->triangles[i++] = 5; m->triangles[i++] = 6; // right
	m->triangles[i++] = 1; m->triangles[i++] = 6; m->triangles[i++] = 2; // right
	m->triangles[i++] = 6; m->triangles[i++] = 3; m->triangles[i++] = 2; // back
	m->triangles[i++] = 6; m->triangles[i++] = 7; m->triangles[i++] = 3; // back
	m->triangles[i++] = 4; m->triangles[i++] = 3; m->triangles[i++] = 7; // left
	m->triangles[i++] = 4; m->triangles[i++] = 0; m->triangles[i++] = 3; // left
	m->triangles[i++] = 4; m->triangles[i++] = 6; m->triangles[i++] = 5; // bottom
	m->triangles[i++] = 4; m->triangles[i++] = 7; m->triangles[i++] = 6; // bottom

	m->CalculateNormals();

	return m;
}

BEMesh* BEModel::AxisMesh()
{
	BEMesh* m = new BEMesh(4, 0);

	m->verticies[0] = { 0,0,0 };
	m->verticies[1] = { 1,0,0 };
	m->verticies[2] = { 0,1,0 };
	m->verticies[3] = { 0,0,1 };

	m->AddLines(3);

	m->lines[0] = 0; m->lines[1] = 1;
	m->lines[2] = 0; m->lines[3] = 2;
	m->lines[4] = 0; m->lines[5] = 3;

	return m;
}
