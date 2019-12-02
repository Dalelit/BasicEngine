#include "BEMeshPrimatives.h"

BEMesh* BEMeshPrimatives::TriangleMesh()
{
	BEMesh* m = new BEMesh(3, 1, BEMesh::BEMeshTopology::TRIANGLE_LIST);

	m->verticies[0] = { { 0,0,0,1 }, {0,0,1,1}, {1,1,1,1} };
	m->verticies[1] = { { 0,1,0,1 }, {0,0,1,1}, {1,1,1,1} };
	m->verticies[2] = { { 1,0,0,1 }, {0,0,1,1}, {1,1,1,1} };

	// clockwise
	m->indicies[0] = 0;
	m->indicies[1] = 1;
	m->indicies[2] = 2;

	m->triangles[0].indx = m->indicies;

	m->CalculateTriangleInfo();

	return m;
}

BEMesh* BEMeshPrimatives::CubeMesh()
{
	BEMesh* m = new BEMesh(6 * 4, 6 * 2, BEMesh::BEMeshTopology::TIRANGLE_INDEX);
	int i = 0;

	// top
	//    3--2
	//   /  /|
	//  0--1 x
	//  |  |/
	//  x--x
	m->verticies[0] = { { -0.5f, 0.5f, 0.5f, 1.0f }, {0,1,0,1}, {1,1,1,1} };
	m->verticies[1] = { { 0.5f, 0.5f, 0.5f, 1.0f }, {0,1,0,1}, {1,1,1,1} };
	m->verticies[2] = { { 0.5f, 0.5f,  -0.5f, 1.0f }, {0,1,0,1}, {1,1,1,1} };
	m->verticies[3] = { { -0.5f, 0.5f,  -0.5f, 1.0f }, {0,1,0,1}, {1,1,1,1} };
	m->indicies[i++] = 0; m->indicies[i++] = 2; m->indicies[i++] = 1;
	m->indicies[i++] = 0; m->indicies[i++] = 3; m->indicies[i++] = 2;

	// front
	//    x--x
	//   /  /|
	//  4--5 x
	//  |  |/
	//  7--6
	m->verticies[4] = { { -0.5f, 0.5f, 0.5f, 1.0f }, {0,0,1,1}, {1,1,1,1} };
	m->verticies[5] = { { 0.5f, 0.5f, 0.5f, 1.0f }, {0,0,1,1}, {1,1,1,1} };
	m->verticies[6] = { { 0.5f, -0.5f, 0.5f, 1.0f }, {0,0,1,1}, {1,1,1,1} };
	m->verticies[7] = { { -0.5f, -0.5f, 0.5f, 1.0f }, {0,0,1,1}, {1,1,1,1} };
	m->indicies[i++] = 7; m->indicies[i++] = 4; m->indicies[i++] = 5;
	m->indicies[i++] = 7; m->indicies[i++] = 5; m->indicies[i++] = 6;

	// right
	//    x--9
	//   /  /|
	//  x--8 10
	//  |  |/
	//  x--11
	m->verticies[8] = { { 0.5f, 0.5f, 0.5f, 1.0f }, {1,0,0,1}, {1,1,1,1} };
	m->verticies[9] = { { 0.5f, 0.5f, -0.5f, 1.0f }, {1,0,0,1}, {1,1,1,1} };
	m->verticies[10] = { { 0.5f, -0.5f, -0.5f, 1.0f }, {1,0,0,1}, {1,1,1,1} };
	m->verticies[11] = { { 0.5f, -0.5f, 0.5f, 1.0f }, {1,0,0,1}, {1,1,1,1} };
	m->indicies[i++] = 8; m->indicies[i++] = 9; m->indicies[i++] = 10;
	m->indicies[i++] = 8; m->indicies[i++] = 10; m->indicies[i++] = 11;

	// back
	//    13-12
	//   /  /|
	//  x--x 15
	//  |  |/
	//  x--x
	m->verticies[12] = { { 0.5f, 0.5f, -0.5f, 1.0f }, {0,0,-1,1}, {1,1,1,1} };
	m->verticies[13] = { { -0.5f, 0.5f, -0.5f, 1.0f }, {0,0,-1,1}, {1,1,1,1} };
	m->verticies[14] = { { -0.5f, -0.5f, -0.5f, 1.0f }, {0,0,-1,1}, {1,1,1,1} };
	m->verticies[15] = { { 0.5f, -0.5f, -0.5f, 1.0f }, {0,0,-1,1}, {1,1,1,1} };
	m->indicies[i++] = 12; m->indicies[i++] = 13; m->indicies[i++] = 14;
	m->indicies[i++] = 12; m->indicies[i++] = 14; m->indicies[i++] = 15;

	// left
	//    16-x
	//   /  /|
	//  17-x x
	//  |  |/
	//  18-x
	m->verticies[16] = { { -0.5f, 0.5f, -0.5f, 1.0f }, {-1,0,0,1}, {1,1,1,1} };
	m->verticies[17] = { { -0.5f, 0.5f, 0.5f, 1.0f }, {-1,0,0,1}, {1,1,1,1} };
	m->verticies[18] = { { -0.5f, -0.5f, 0.5f, 1.0f }, {-1,0,0,1}, {1,1,1,1} };
	m->verticies[19] = { { -0.5f, -0.5f, -0.5f, 1.0f }, {-1,0,0,1}, {1,1,1,1} };
	m->indicies[i++] = 16; m->indicies[i++] = 17; m->indicies[i++] = 18;
	m->indicies[i++] = 16; m->indicies[i++] = 18; m->indicies[i++] = 19;

	// bottom
	//    x--x
	//   /  /|
	//  x--x 22
	//  |  |/
	//  20-21
	m->verticies[20] = { { -0.5f, -0.5f, 0.5f, 1.0f }, {0,-1,0,1}, {1,1,1,1} };
	m->verticies[21] = { { 0.5f, -0.5f, 0.5f, 1.0f }, {0,-1,0,1}, {1,1,1,1} };
	m->verticies[22] = { { 0.5f, -0.5f, -0.5f, 1.0f }, {0,-1,0,1}, {1,1,1,1} };
	m->verticies[23] = { { -0.5f, -0.5f, -0.5f, 1.0f }, {0,-1,0,1}, {1,1,1,1} };
	m->indicies[i++] = 20; m->indicies[i++] = 21; m->indicies[i++] = 22;
	m->indicies[i++] = 20; m->indicies[i++] = 22; m->indicies[i++] = 23;

	m->CalculateTriangleInfo();

	return m;
}

//BEMesh* BEMeshPrimatives::AxisMesh()
//{
//	BEMesh* m = new BEMesh(4, 0, BEMesh::BEMeshTopology::LINES);
//
//	m->verticies[0] = { 0,0,0,1 };
//	m->verticies[1] = { 1,0,0,1 };
//	m->verticies[2] = { 0,1,0,1 };
//	m->verticies[3] = { 0,0,1,1 };
//
//	m->AddLines(3);
//
//	m->lines[0] = 0; m->lines[1] = 1;
//	m->lines[2] = 0; m->lines[3] = 2;
//	m->lines[4] = 0; m->lines[5] = 3;
//
//	return m;
//}
