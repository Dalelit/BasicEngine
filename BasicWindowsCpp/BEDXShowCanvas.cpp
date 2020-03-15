#include "pch.h"
#include "BEDXShowCanvas.h"
#include "BEDXVertexShader.h"

using namespace DirectX;

BEDXShowCanvas::BEDXShowCanvas(BEDirectXDevice& device, BESurface2D<XMVECTOR>& surface, bool updateable)
{
	struct PosTexVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
	};

	BEDXVertexShaderLayout vsLayout = {
		{"Position", DXGI_FORMAT_R32G32B32_FLOAT},
		{"Texcoord", DXGI_FORMAT_R32G32_FLOAT}
	};


	BETexVertex verticies[6];

	if (updateable)
		pTexUpdt = std::make_unique<BEDXTextureUpdateable>(device, surface, DXGI_FORMAT_R32G32B32A32_FLOAT);
	else
		pTex = std::make_unique<BEDXTexture>(device, surface, DXGI_FORMAT_R32G32B32A32_FLOAT);

	// set the verticies
	verticies[0] = { { -1,-1,0 }, {0,0} };
	verticies[1] = { { -1, 1,0 }, {0,1} };
	verticies[2] = { {  1, 1,0 }, {1,1} };

	verticies[3] = { { -1,-1,0 }, {0,0} };
	verticies[4] = { {  1, 1,0 }, {1,1} };
	verticies[5] = { {  1,-1,0 }, {1,0} };

	// create the vertex buffer
	vertexBuffer = std::make_unique<BEDXVertexBuffer>(device, &verticies, (unsigned int)ARRAYSIZE(verticies), (unsigned int)sizeof(PosTexVertex));
	vertexBuffer->Bind(device);

	// create the vertex shader

	std::string vsCode = R"(
	struct VSOut
	{
		float2 tc : Texcoord;
		float4 pos : SV_POSITION;
	};

	VSOut main(float3 pos : Position, float2 tc : Texcoord)
	{
		VSOut vso;

		vso.pos = float4(pos, 1.0f);
		vso.tc = tc;

		return vso;
	})";

	vertexShader = std::make_unique<BEDXVertexShader>(device, vsCode, vsLayout);
	vertexShader->Bind(device);

	std::string psCode = R"(
		Texture2D tex;
		SamplerState smplr;

		float4 main(float2 tc : Texcoord) : SV_TARGET
		{
			return float4(tex.Sample(smplr, tc).rgb, 1.0f);
		})";

	// create the pixel shader
	pixelShader = std::make_unique<BEDXPixelShaderSource>(device, psCode);
	pixelShader->Bind(device);
}

void BEDXShowCanvas::Draw(BEDirectXDevice& device)
{
	if (pTexUpdt)
	{
		pTexUpdt->UpdateFromSource(device);
		pTexUpdt->Bind(device);
	}
	else
	{
		pTex->Bind(device);
	}

	vertexBuffer->Draw(device);
}
