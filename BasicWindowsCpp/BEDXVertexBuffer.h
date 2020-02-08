#pragma once
#include "BEMesh.h"
#include "BEDirectXResource.h"
#include <DirectXMath.h>

class BEDXVertexBuffer : BEDirectXResource
{
public:

	BEDXVertexBuffer(BEDirectXDevice& device, BEMesh* pMesh);
	BEDXVertexBuffer(BEDirectXDevice& device, void* pVerticies, unsigned int numberVerticies, unsigned int vertexSize);

	virtual void Bind(BEDirectXDevice& device);
	virtual void Draw(BEDirectXDevice& device);

protected:

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT bufferStrides[1] = {  }; // set by vertexSize in constructor
	UINT bufferOffsets[1] = { 0 };

	unsigned int vertCount  = 0; // number of verticies to draw

};

class BEDXVertexBufferIndexed : public BEDXVertexBuffer
{
public:

	BEDXVertexBufferIndexed(BEDirectXDevice& device, BEMesh* pMesh);
	
	void Bind(BEDirectXDevice& device);
	void Draw(BEDirectXDevice& device);

protected:
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
	unsigned int indxCount = 0; // number of indexes to draw
};