#pragma once

#include "BEDirectXDrawable.h"
#include "BEScene.h" // to do : temp
#include <DirectXMath.h>

class BEDXVertexBuffer : public BEDirectXDrawable
{
public:

	struct BEVertex {
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR color;
	};

	BEDXVertexBuffer(BEDirectXDevice& device, BEMesh* pMesh, XMVECTOR color); // to do: change color
	void Draw(BEDirectXDevice& device);

private:
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer = NULL;
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer = NULL;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT bufferStrides[1] = { sizeof(BEVertex) };
	UINT bufferOffsets[1] = { 0 };

	unsigned int vertCount = 0; // number of verticies to draw
	unsigned int indxCount = 0; // number of indexes to draw
};

