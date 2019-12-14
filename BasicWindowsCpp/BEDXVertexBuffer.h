#pragma once

#include "BEDirectXDrawable.h"
#include "BEScene.h" // to do : temp
#include <DirectXMath.h>

class BEDXVertexBuffer : public BEDirectXDrawable
{
public:

	BEDXVertexBuffer(BEDirectXDevice& device, BEMesh* pMesh, unsigned int vertexSize);
	BEDXVertexBuffer(BEDirectXDevice& device, void* pVerticies, unsigned int numberVerticies, unsigned int vertexSize);
	void Draw(BEDirectXDevice& device);

private:
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT bufferStrides[1] = {  }; // set by vertexSize in constructor
	UINT bufferOffsets[1] = { 0 };

	unsigned int vertCount  = 0; // number of verticies to draw
	unsigned int indxCount  = 0; // number of indexes to draw
};

