#pragma once

#include "BEDirectXDrawable.h"
#include "BEMesh.h"
#include <DirectXMath.h>

class BEDXVertexBuffer : public BEDirectXDrawableResource
{
public:

	BEDXVertexBuffer(BEDirectXDevice& device, BEMesh* pMesh, unsigned int vertexSize = (unsigned int)sizeof(BEVertex));
	BEDXVertexBuffer(BEDirectXDevice& device, void* pVerticies, unsigned int numberVerticies, unsigned int vertexSize = (unsigned int)sizeof(BEVertex));
	void Bind(BEDirectXDevice& device);
	void Draw(BEDirectXDevice& device);

protected:
	BEDXVertexBuffer() = default;

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT bufferStrides[1] = {  }; // set by vertexSize in constructor
	UINT bufferOffsets[1] = { 0 };

	unsigned int vertCount  = 0; // number of verticies to draw

};

class BEDXVertexBufferIndexed : public BEDXVertexBuffer
{
public:

	BEDXVertexBufferIndexed(BEDirectXDevice& device, BEMesh* pMesh, unsigned int vertexSize = (unsigned int)sizeof(BEVertex));
	
	void Bind(BEDirectXDevice& device);
	void Draw(BEDirectXDevice& device);

protected:
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	unsigned int indxCount = 0; // number of indexes to draw
};