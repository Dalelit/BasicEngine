#include "BEDXVertexBuffer.h"

#define BEDIRECTX_VERT_BUFFER_MAXSIZE 1000

BEDXVertexBuffer::BEDXVertexBuffer(BEDirectXDevice& device, BEWorld* pWorld)
{
	HRESULT hr;

	BEVertex* verticies = new BEVertex[BEDIRECTX_VERT_BUFFER_MAXSIZE];

	// load a vertex buffer with data from the world model.
	// To do: this is a temp hack.
	UINT vertIndx = 0;

	for (UINT eIndx = 0; eIndx < pWorld->entityCount; eIndx++)
	{
		BEMesh* m = pWorld->entities[eIndx]->mesh;

		if (m)
		{
			for (UINT i = 0; i < m->vCount; i++)
			{
				// To Do: efficient way to do this?
				verticies[vertIndx].position = m->verticies[i];
				verticies[vertIndx].color = pWorld->entities[eIndx]->color;
				vertIndx++;
			}
		}
	}
	vertCount = vertIndx;

	// create triangle data
	D3D11_SUBRESOURCE_DATA triangleData = {};
	triangleData.SysMemPitch = 0;
	triangleData.SysMemSlicePitch = 0;
	triangleData.pSysMem = verticies;

	// create the triangle buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(BEVertex) * vertCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(BEVertex);

	hr = device.pDevice->CreateBuffer(&bufferDesc, &triangleData, &pTriangleBuffer);

	delete verticies; // to do: sort out this buffer clean up.
	
	BEDXRESOURCE_ERRORCHECK(hr)
}

void BEDXVertexBuffer::Draw(BEDirectXDevice& device)
{
	// set the triangle vertex buffer
	device.pImmediateContext->IASetVertexBuffers(0, 1, pTriangleBuffer.GetAddressOf(), bufferStrides, bufferOffsets);
	device.pImmediateContext->IASetPrimitiveTopology(topology);
	device.pImmediateContext->Draw(vertCount, 0u);
}
