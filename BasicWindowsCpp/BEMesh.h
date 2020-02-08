#pragma once
#include "BEBounds.h"
#include "BESampler.h"
#include "BEMaterial.h"
#include <vector>
#include <string>

// Notes/Reminders/To check
// - RH coord system... sticking with this? DX is LH
// - traingle ordered in clockwise direction

class BEMesh
{
public:

	enum class BEMeshTopology
	{
		TRIANGLE_LIST,
		TRIANGLE_INDEX
	};

	BEMeshTopology topology = BEMeshTopology::TRIANGLE_LIST;
	BEMaterial material;

	std::wstring name;

	BEVertex* verticies = nullptr;
	unsigned int vertCount = 0;
	unsigned int GetVertexSize() { return (unsigned int)sizeof(BEVertex); }

	unsigned int* indicies = nullptr;
	unsigned int indxCount = 0;

	DirectX::XMVECTOR defaultColor = {1,1,1,1};

	BEMesh(unsigned int _vertCount, unsigned int _triCount, BEMeshTopology _topology);
	BEMesh(std::vector<DirectX::XMFLOAT3> verticies, std::vector<DirectX::XMFLOAT3> normals);
	BEMesh(std::vector<DirectX::XMFLOAT3> verticies, std::vector<DirectX::XMFLOAT3> normals, std::vector<DirectX::XMFLOAT2> texcoord, std::vector<unsigned int> index);
	BEMesh(std::vector<BEVertex> srcVerticies);
	~BEMesh();

	inline void SetColor(DirectX::XMVECTOR color) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].color = color; }

	inline void Translate(DirectX::XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorAdd(verticies[i].position, v); }
	inline void Scale(float f) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorScale(verticies[i].position, f); }
	inline void Scale(DirectX::XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorMultiply(verticies[i].position, v); }

	inline void Transform(DirectX::XMMATRIX m) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVector3Transform(verticies[i].position, m); }

	BEBoundsBox bounds;
	//BEBoundsSphere bounds;
	void SetBounds() { bounds.SetBounds(vertCount, verticies); };

	void SwapIndexOrder();
};
