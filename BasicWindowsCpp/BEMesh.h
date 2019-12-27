#pragma once
#include "BECommon.h"
#include "BEVertex.h"
#include <vector>

using namespace DirectX;

// Notes/Reminders/To check
// - RH coord system... sticking with this? DX is LH
// - traingle ordered in clockwise direction

class BEMesh
{
public:

	enum class BEMeshTopology
	{
		TRIANGLE_LIST,
		TRIANGLE_INDEX,
		LINES
	};

	BEMeshTopology topology = BEMeshTopology::TRIANGLE_LIST;

	BEVertex* verticies = nullptr;
	unsigned int vertCount = 0;

	unsigned int* indicies = nullptr;
	unsigned int indxCount = 0;

	BETriangle* triangles = nullptr;
	unsigned int triCount = 0;

	unsigned int lCount = 0;
	unsigned int lBufferSize = 0;
	unsigned int* lines = nullptr;

	BEMesh(unsigned int _vertCount, unsigned int _triCount, BEMeshTopology _topology);
	BEMesh(std::vector<XMFLOAT3> verticies, std::vector<XMFLOAT3> normals, BEMeshTopology _topology);
	~BEMesh();

	void AddLines(unsigned int _lCount);

	void CalculateTriangleInfo();

	inline void SetColor(XMVECTOR color) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].color = color; }

	inline void Translate(XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position += v; }
	inline void Scale(float f) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position *= f; }
	inline void Scale(XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position *= v; }

	inline void Transform(XMMATRIX m) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = XMVector3Transform(verticies[i].position, m); }

};
