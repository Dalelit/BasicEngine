#pragma once
#include "BECommon.h"
#include "BEBounds.h"
#include "BESampler.h"
#include <vector>

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
	BESampler* pTextureSampler = nullptr;

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
	BEMesh(std::vector<DirectX::XMFLOAT3> verticies, std::vector<DirectX::XMFLOAT3> normals, BEMeshTopology _topology);
	~BEMesh();

	void AddLines(unsigned int _lCount);

	void CalculateTriangleInfo();

	inline void SetColor(DirectX::XMVECTOR color) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].color = color; }

	inline void Translate(DirectX::XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorAdd(verticies[i].position, v); }
	inline void Scale(float f) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorScale(verticies[i].position, f); }
	inline void Scale(DirectX::XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorMultiply(verticies[i].position, v); }

	inline void Transform(DirectX::XMMATRIX m) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVector3Transform(verticies[i].position, m); }

	inline bool IsTextured() { return pTextureSampler != nullptr; }

	BEBoundsBox bounds;
	//BEBoundsSphere bounds;
	void SetBounds() { bounds.SetBounds(vertCount, verticies); };
};
