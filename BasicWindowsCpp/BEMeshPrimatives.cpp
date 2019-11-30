#include "BEMeshPrimatives.h"

BEMesh* BEMeshPrimatives::TriangleMesh()
{
	BEMesh* m = new BEMesh(3, 1, BEMesh::BEMeshTopology::TRIANGLE_LIST);

	m->verticies[0] = { { 0,0,0,1 }, {1,1,1,1} };
	m->verticies[1] = { { 0,1,0,1 }, {1,1,1,1} };
	m->verticies[2] = { { 1,0,0,1 }, {1,1,1,1} };

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
	BEMesh* m = new BEMesh(8, 12, BEMesh::BEMeshTopology::TIRANGLE_INDEX);

	//    3--2
	//   /  /|
	//  0--1 6
	//  |  |/
	//  4--5


	m->verticies[0] = {{ -0.5f, 0.5f, 0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[1] = {{ 0.5f, 0.5f, 0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[2] = {{ 0.5f, 0.5f,  -0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[3] = {{ -0.5f, 0.5f,  -0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[4] = {{ -0.5f, -0.5f, 0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[5] = {{ 0.5f, -0.5f, 0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[6] = {{ 0.5f, -0.5f,  -0.5f, 1.0f }, {1,1,1,1} };
	m->verticies[7] = {{ -0.5f, -0.5f,  -0.5f, 1.0f }, {1,1,1,1} };
	
	int i = 0;

	// to do: check if clockwise is a good direction

	// clockwise
	m->indicies[i++] = 0; m->indicies[i++] = 2; m->indicies[i++] = 1; // top
	m->indicies[i++] = 0; m->indicies[i++] = 3; m->indicies[i++] = 2; // top
	m->indicies[i++] = 4; m->indicies[i++] = 1; m->indicies[i++] = 5; // front
	m->indicies[i++] = 4; m->indicies[i++] = 0; m->indicies[i++] = 1; // front
	m->indicies[i++] = 1; m->indicies[i++] = 6; m->indicies[i++] = 5; // right
	m->indicies[i++] = 1; m->indicies[i++] = 2; m->indicies[i++] = 6; // right
	m->indicies[i++] = 6; m->indicies[i++] = 2; m->indicies[i++] = 3; // back
	m->indicies[i++] = 6; m->indicies[i++] = 3; m->indicies[i++] = 7; // back
	m->indicies[i++] = 4; m->indicies[i++] = 7; m->indicies[i++] = 3; // left
	m->indicies[i++] = 4; m->indicies[i++] = 3; m->indicies[i++] = 0; // left
	m->indicies[i++] = 4; m->indicies[i++] = 5; m->indicies[i++] = 6; // bottom
	m->indicies[i++] = 4; m->indicies[i++] = 6; m->indicies[i++] = 7; // bottom

	// counter clockwise
	//m->indicies[i++] = 0; m->indicies[i++] = 1; m->indicies[i++] = 2; // top
	//m->indicies[i++] = 0; m->indicies[i++] = 2; m->indicies[i++] = 3; // top
	//m->indicies[i++] = 4; m->indicies[i++] = 5; m->indicies[i++] = 1; // front
	//m->indicies[i++] = 4; m->indicies[i++] = 1; m->indicies[i++] = 0; // front
	//m->indicies[i++] = 1; m->indicies[i++] = 5; m->indicies[i++] = 6; // right
	//m->indicies[i++] = 1; m->indicies[i++] = 6; m->indicies[i++] = 2; // right
	//m->indicies[i++] = 6; m->indicies[i++] = 3; m->indicies[i++] = 2; // back
	//m->indicies[i++] = 6; m->indicies[i++] = 7; m->indicies[i++] = 3; // back
	//m->indicies[i++] = 4; m->indicies[i++] = 3; m->indicies[i++] = 7; // left
	//m->indicies[i++] = 4; m->indicies[i++] = 0; m->indicies[i++] = 3; // left
	//m->indicies[i++] = 4; m->indicies[i++] = 6; m->indicies[i++] = 5; // bottom
	//m->indicies[i++] = 4; m->indicies[i++] = 7; m->indicies[i++] = 6; // bottom

	m->CalculateTriangleInfo();

	return m;
}

BEMesh* BEMeshPrimatives::AxisMesh()
{
	BEMesh* m = new BEMesh(4, 0, BEMesh::BEMeshTopology::LINES);

	m->verticies[0] = { 0,0,0,1 };
	m->verticies[1] = { 1,0,0,1 };
	m->verticies[2] = { 0,1,0,1 };
	m->verticies[3] = { 0,0,1,1 };

	m->AddLines(3);

	m->lines[0] = 0; m->lines[1] = 1;
	m->lines[2] = 0; m->lines[3] = 2;
	m->lines[4] = 0; m->lines[5] = 3;

	return m;
}
