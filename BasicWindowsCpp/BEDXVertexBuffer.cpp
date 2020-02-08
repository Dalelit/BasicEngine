#include "BEDXVertexBuffer.h"

// To do: template this?


BEDXVertexBuffer::BEDXVertexBuffer(BEDirectXDevice& device, BEMesh* pMesh, unsigned int vertexSize)
{
	HRESULT hr;

	bufferStrides[0] = vertexSize;

	vertCount = pMesh->vertCount;

	// create triangle data
	D3D11_SUBRESOURCE_DATA data = {};
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	data.pSysMem = pMesh->verticies;

	// create the triangle buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = vertexSize * vertCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = vertexSize;

	hr = device.pDevice->CreateBuffer(&bufferDesc, &data, &pVertexBuffer);

	BEDXRESOURCE_ERRORCHECK(hr);
}

BEDXVertexBuffer::BEDXVertexBuffer(BEDirectXDevice& device, void* pVerticies, unsigned int numberVerticies, unsigned int vertexSize)
{
	HRESULT hr;

	bufferStrides[0] = vertexSize;

	vertCount = numberVerticies;

	// create triangle data
	D3D11_SUBRESOURCE_DATA data = {};
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	data.pSysMem = pVerticies;

	// create the triangle buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = vertexSize * vertCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = vertexSize;

	hr = device.pDevice->CreateBuffer(&bufferDesc, &data, &pVertexBuffer);

	BEDXRESOURCE_ERRORCHECK(hr);
}

void BEDXVertexBuffer::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->IASetPrimitiveTopology(topology);
	device.pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), bufferStrides, bufferOffsets);
}

void BEDXVertexBuffer::Draw(BEDirectXDevice& device)
{
	device.pImmediateContext->Draw(vertCount, 0u);
}

BEDXVertexBufferIndexed::BEDXVertexBufferIndexed(BEDirectXDevice& device, BEMesh* pMesh, unsigned int vertexSize) :
	BEDXVertexBuffer::BEDXVertexBuffer(device, pMesh, vertexSize)
{
	HRESULT hr;

	// Add an index buffer.
	indxCount = pMesh->indxCount;

	D3D11_SUBRESOURCE_DATA data = {};
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	data.pSysMem = pMesh->indicies;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(unsigned int) * indxCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(unsigned int);

	hr = device.pDevice->CreateBuffer(&bufferDesc, &data, &pIndexBuffer);

	BEDXRESOURCE_ERRORCHECK(hr);
}

void BEDXVertexBufferIndexed::Bind(BEDirectXDevice& device)
{
	device.pImmediateContext->IASetPrimitiveTopology(topology);
	device.pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), bufferStrides, bufferOffsets);
	device.pImmediateContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

void BEDXVertexBufferIndexed::Draw(BEDirectXDevice& device)
{
	device.pImmediateContext->DrawIndexed(indxCount, 0u, 0u);
}
