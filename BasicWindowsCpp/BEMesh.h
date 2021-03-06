#pragma once
#include "pch.h"
#include "BEBounds.h"
#include "BESampler.h"
#include "BEMaterial.h"

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

	std::string name;

	BEVertex* verticies = nullptr;
	unsigned int vertCount = 0;
	unsigned int GetVertexSize() { return (unsigned int)sizeof(BEVertex); }

	unsigned int* indicies = nullptr;
	unsigned int indxCount = 0;

	DirectX::XMVECTOR defaultColor = {1,1,1,1};

	BEMesh(unsigned int _vertCount, unsigned int _triCount, BEMeshTopology _topology);
	BEMesh(std::vector<DirectX::XMFLOAT3> verticies, std::vector<DirectX::XMFLOAT3> normals);
	BEMesh(std::vector<DirectX::XMVECTOR> verticies, std::vector<DirectX::XMVECTOR> normals, std::vector<unsigned int> index);
	BEMesh(std::vector<DirectX::XMFLOAT3> verticies, std::vector<DirectX::XMFLOAT3> normals, std::vector<DirectX::XMFLOAT2> texcoord, std::vector<unsigned int> index);
	BEMesh(std::vector<BEVertex> srcVerticies);
	~BEMesh();

	inline void SetVertexColor(DirectX::XMVECTOR color) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].color = color; }
	inline void SetColor(DirectX::XMVECTOR color) { material.diffuseColor = color; material.ambientColor = color; SetVertexColor(color); }

	inline void Translate(DirectX::XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorAdd(verticies[i].position, v); SetBounds(); }
	inline void Scale(float f) { Scale({f, f, f, 1.0f}); }
	inline void Scale(DirectX::XMVECTOR v) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVectorMultiply(verticies[i].position, v);  SetBounds(); }

	inline void Transform(DirectX::XMMATRIX m) { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position = DirectX::XMVector3Transform(verticies[i].position, m); SetBounds(); }

	inline void SetWTo1() { for (unsigned int i = 0; i < vertCount; i++) verticies[i].position.m128_f32[3] = 1.0f; }

	BEBoundsBox bounds;
	//BEBoundsSphere bounds;
	void SetBounds() { bounds.SetBounds(vertCount, verticies); };

	void SwapIndexOrder();
};
