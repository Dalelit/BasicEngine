#include "pch.h"
#include "BEMeshPrimatives.h"
#include "BELogger.h"

using namespace DirectX;

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

	m->SetBounds();

	return m;
}

BEMesh* BEMeshPrimatives::CubeMesh()
{
	BEMesh* m = new BEMesh(6 * 4, 6 * 2, BEMesh::BEMeshTopology::TRIANGLE_INDEX);
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

	m->SetBounds();

	return m;
}

BEMesh* BEMeshPrimatives::Ground(float width, float depth, unsigned int segmentsWide, unsigned int segmentsDeep, float heightMin, float heightMax)
{
	// to do : assert height min max

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<float> dist(heightMin, heightMax);

	unsigned int vertCount = (segmentsWide + 1) * (segmentsDeep + 1);
	unsigned int triCount = segmentsWide * segmentsDeep * 2;

	float xstart = -((float)width / 2.0f);
	float zstart = -((float)segmentsDeep / 2.0f);

	float dx = width / (float)segmentsWide;
	float dz = depth / (float)segmentsDeep;

	XMFLOAT2 texcoord = { 0.0f, 0.0f };
	float texturedx = 1.0f / segmentsWide;
	float texturedy = 1.0f / segmentsDeep;

	BEMesh* m = new BEMesh(vertCount, triCount, BEMesh::BEMeshTopology::TRIANGLE_INDEX);

	DirectX::XMFLOAT4 position = { xstart, 0.0f, zstart, 1.0f };
	DirectX::XMFLOAT4 color = { 0.1f, 0.8f, 0.3f, 1.0f };
	XMVECTOR normal = { 0,1,0,0 };

	for (unsigned int z = 0; z < segmentsDeep+1; z++)
	{
		position.x = xstart;
		texcoord.x = 0.0f;

		for (unsigned int x = 0; x < segmentsWide+1; x++)
		{
			unsigned int indx = z * (segmentsWide+1) + x;
			position.y = dist(gen); // randomise height
			m->verticies[indx].position = XMLoadFloat4(&position);
			m->verticies[indx].normal = normal;
			m->verticies[indx].color = XMLoadFloat4(&color);
			m->verticies[indx].texcoord = texcoord;
			position.x += dx;
			texcoord.x += texturedx;
		}

		position.z += dz;
		texcoord.y += texturedy;
	}

	unsigned int indx = 0;
	for (unsigned int z = 0; z < segmentsDeep; z++)
	{
		for (unsigned int x = 0; x < segmentsWide; x++)
		{
			unsigned int v0indx = z * (segmentsDeep + 1) + x;
			m->indicies[indx++] = v0indx;
			m->indicies[indx++] = v0indx + 1;
			m->indicies[indx++] = v0indx + segmentsWide + 2;

			m->indicies[indx++] = v0indx;
			m->indicies[indx++] = v0indx + segmentsWide + 2;
			m->indicies[indx++] = v0indx + segmentsWide + 1;
		}
	}

	unsigned int stride = segmentsWide + 1;
	for (unsigned int z = 1; z < segmentsDeep; z++)
	{
		for (unsigned int x = 1; x < segmentsWide; x++)
		{
			XMVECTOR pointC = m->verticies[z * stride + x].position;
			XMVECTOR point0 = m->verticies[(z+1) * stride + x].position;
			XMVECTOR point1 = m->verticies[z * stride + x+1].position;
			XMVECTOR point2 = m->verticies[(z-1) * stride + x].position;
			XMVECTOR point3 = m->verticies[z * stride + x-1].position;

			normal += XMVector3Normalize(XMVector3Cross((point0 - pointC), (point1 - pointC)));
			normal += XMVector3Normalize(XMVector3Cross((point1 - pointC), (point2 - pointC)));
			normal += XMVector3Normalize(XMVector3Cross((point2 - pointC), (point3 - pointC)));
			normal += XMVector3Normalize(XMVector3Cross((point3 - pointC), (point0 - pointC)));
			normal *= 0.25f;

			m->verticies[z * segmentsWide + x].normal = normal;
		}
	}

	m->SetBounds();

	return m;
}

// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
BEMesh* BEMeshPrimatives::Icosphere(unsigned int iterations)
{
	std::vector<XMVECTOR> verticies;
	std::vector<unsigned int> indicies;
	std::vector<XMVECTOR> normals;
	
	// note - swapping the order of the indicies from the code sample
	auto addTriangleIndicies = [&indicies](unsigned int i0, unsigned int i1, unsigned int i2) mutable {
		indicies.push_back(i0);
		indicies.push_back(i2);
		indicies.push_back(i1);
	};

	// Create a new vertex between 2 others. Adjust to unit sphere.
	// Return the index of the created vertex
	auto addMidpointVertex = [&verticies](unsigned int i0, unsigned int i1) mutable -> unsigned int {
		XMVECTOR v0 = verticies[i0];
		XMVECTOR v1 = verticies[i1];
		XMVECTOR vmid = XMVector3Normalize((v0 + v1) / 2.0f);

		return AddUniqueVertex(verticies, vmid);
	};


	// create 12 vertices of a icosahedron
	float t = (1.0f + sqrtf(5.0f)) / 2.0f;

	verticies.push_back(XMVector3Normalize({-1.0f, t, 0.0f, 1.0f}));
	verticies.push_back(XMVector3Normalize({1.0f, t, 0.0f, 1.0f}));
	verticies.push_back(XMVector3Normalize({-1.0f, -t, 0.0f, 1.0f}));
	verticies.push_back(XMVector3Normalize({1.0f, -t, 0.0f, 1.0f}));

	verticies.push_back(XMVector3Normalize({0.0f, -1.0f, t, 1.0f}));
	verticies.push_back(XMVector3Normalize({0.0f, 1.0f, t, 1.0f}));
	verticies.push_back(XMVector3Normalize({0.0f, -1.0f, -t, 1.0f}));
	verticies.push_back(XMVector3Normalize({0.0f, 1.0f, -t, 1.0f}));

	verticies.push_back(XMVector3Normalize({t, 0.0f, -1.0f, 1.0f}));
	verticies.push_back(XMVector3Normalize({t, 0.0f, 1.0f, 1.0f}));
	verticies.push_back(XMVector3Normalize({-t, 0.0f, -1.0f, 1.0f}));
	verticies.push_back(XMVector3Normalize({-t, 0.0f, 1.0f, 1.0f}));

	// create 20 triangles of the icosahedron

	// 5 faces around point 0
	addTriangleIndicies(0, 11, 5);
	addTriangleIndicies(0, 5, 1);
	addTriangleIndicies(0, 1, 7);
	addTriangleIndicies(0, 7, 10);
	addTriangleIndicies(0, 10, 11);

	// 5 adjacent faces
	addTriangleIndicies(1, 5, 9);
	addTriangleIndicies(5, 11, 4);
	addTriangleIndicies(11, 10, 2);
	addTriangleIndicies(10, 7, 6);
	addTriangleIndicies(7, 1, 8);

	// 5 faces around point 3
	addTriangleIndicies(3, 9, 4);
	addTriangleIndicies(3, 4, 2);
	addTriangleIndicies(3, 2, 6);
	addTriangleIndicies(3, 6, 8);
	addTriangleIndicies(3, 8, 9);

	// 5 adjacent faces
	addTriangleIndicies(4, 9, 5);
	addTriangleIndicies(2, 4, 11);
	addTriangleIndicies(6, 2, 10);
	addTriangleIndicies(8, 6, 7);
	addTriangleIndicies(9, 8, 1);

	// created the core object.

	// now iterate over the triangles dividing them.
	for (unsigned int iter = 0; iter < iterations; iter++)
	{
		auto current = indicies.begin();
		auto end = indicies.end();

		std::vector<unsigned int> newIndicies;

		while (current != end)
		{
			unsigned int i0 = *current;
			unsigned int i1 = *(current + 1);
			unsigned int i2 = *(current + 2);

			unsigned int i01 = addMidpointVertex(i0, i1);
			unsigned int i12 = addMidpointVertex(i1, i2);
			unsigned int i20 = addMidpointVertex(i2, i0);

			// change the current triangle to be a sub triangle
			// current = i0; // unchanged
			current++;
			*current = i01;
			current++;
			*current = i20;
			current++;

			// add the 3 new triangles
			newIndicies.push_back(i01); newIndicies.push_back(i1); newIndicies.push_back(i12);
			newIndicies.push_back(i01); newIndicies.push_back(i12); newIndicies.push_back(i20);
			newIndicies.push_back(i20); newIndicies.push_back(i12); newIndicies.push_back(i2);
		}

		// add the new ones to the indicies
		for (auto indx : newIndicies) indicies.push_back(indx);
	}


	// calc normals
	for (auto v : verticies)
	{
		XMVECTOR n = XMVector3Normalize(v);
		normals.push_back(n);
	}

	// to do: untested
	// Calc texture uv
	//for (auto n : normals)
	//{
	//	float tu = asinf(n.m128_f32[0]) / XM_PI + 0.5f;
	//	float tv = asinf(n.m128_f32[1]) / XM_PI + 0.5f;
	//}

	BEMesh* m = new BEMesh(verticies, normals, indicies);

	m->SetBounds();

	return m;
}

unsigned int BEMeshPrimatives::AddUniqueVertex(std::vector<XMVECTOR>& verticies, XMVECTOR newVertex)
{
	auto current = verticies.begin();
	auto end = verticies.end();
	unsigned int indx = 0;

	while (current != end && !XMVector3Equal(*current, newVertex))
	{
		current++;
		indx++;
	}

	if (current == end) verticies.push_back(newVertex);

	return indx;
}
