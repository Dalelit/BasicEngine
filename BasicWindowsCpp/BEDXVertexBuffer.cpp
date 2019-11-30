#include "BEDXVertexBuffer.h"

BEDXVertexBuffer::BEDXVertexBuffer(BEDirectXDevice& device, BEMesh* pMesh)
{
	HRESULT hr;

	vertCount = pMesh->vertCount;

	// create triangle data
	D3D11_SUBRESOURCE_DATA data = {};
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	data.pSysMem = pMesh->verticies;

	// create the triangle buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(BEVertex) * vertCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(BEVertex);

	hr = device.pDevice->CreateBuffer(&bufferDesc, &data, &pVertexBuffer);

	BEDXRESOURCE_ERRORCHECK(hr);

	// If if is an index list, then create an index buffer.
	if (pMesh->topology == BEMesh::BEMeshTopology::TIRANGLE_INDEX)
	{
		indxCount = pMesh->indxCount;

		//D3D11_SUBRESOURCE_DATA data = {};
		//data.SysMemPitch = 0;
		//data.SysMemSlicePitch = 0;
		data.pSysMem = pMesh->indicies;

		//D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(unsigned int) * indxCount;
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//bufferDesc.CPUAccessFlags = 0;
		//bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = sizeof(unsigned int);

		hr = device.pDevice->CreateBuffer(&bufferDesc, &data, &pIndexBuffer);

		BEDXRESOURCE_ERRORCHECK(hr);
	}


}

void BEDXVertexBuffer::Draw(BEDirectXDevice& device)
{
	device.pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), bufferStrides, bufferOffsets);
	device.pImmediateContext->IASetPrimitiveTopology(topology);

	if (pIndexBuffer)
	{
		device.pImmediateContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
		device.pImmediateContext->DrawIndexed(indxCount, 0u, 0u);
	}
	else
	{
		device.pImmediateContext->Draw(vertCount, 0u);
	}
	
}
