#pragma once
#include "BERenderPipeline.h"
#include <sstream>


struct BEPipelineVSData
{
	DirectX::XMVECTOR positionWS;
	DirectX::XMVECTOR normalWS;
	DirectX::XMVECTOR positionSS;
	DirectX::XMVECTOR color;
	DirectX::XMFLOAT2 texcoord;

	inline BEPipelineVSData operator+(const BEPipelineVSData& rhs);
	inline BEPipelineVSData operator-(const BEPipelineVSData& rhs);
	inline BEPipelineVSData operator*(const float rhs);
	inline BEPipelineVSData operator/(const float rhs);
	inline void operator+=(const BEPipelineVSData& rhs);
	inline void operator*=(const float rhs);
	inline void operator/=(const float rhs);
};

struct BEPipelineVSConstants
{
	BEModel* pModel;
	BEEntity* pEntity;
};

struct BEPipelinePSData
{
	DirectX::XMVECTOR positionWS;
	DirectX::XMVECTOR normalWS;
	DirectX::XMVECTOR color;
	DirectX::XMFLOAT2 texcoord;
	BEModel* pModel;
	BEEntity* pEntity;

	BEPipelinePSData() = default;

	BEPipelinePSData(BEPipelineVSData& rhs) :
		positionWS(rhs.positionWS),
		normalWS(rhs.normalWS),
		color(rhs.color),
		texcoord(rhs.texcoord)
		//pModel = nullptr;
		//pEntity = nullptr;
		{}

	inline void operator=(BEPipelineVSData& rhs) {
		positionWS = rhs.positionWS;
		normalWS = rhs.normalWS;
		color = rhs.color;
		texcoord = rhs.texcoord;
	}
};

class BERenderProgrammablePipeline
{
public:
	BERenderProgrammablePipeline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderProgrammablePipeline();

	void Draw();

	void Clear();

	void VertexShading(BEPipelineVSConstants& constants);
	void VertexShader(BEPipelineVSConstants& constants, BEVertex* pVertex, BEPipelineVSData* pOutput);

	void GeometryShader(BEPipelineVSConstants& constants);
	void CullClipDraw(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	
	void RasterizerPoints(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void RasterizerWireframe(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void RasterizerTriangle(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);

	void PixelShading();
	void PixelShaderFull(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput);
	void PixelShaderColorLight(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput);
	void PixelShaderColor(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput);

	// method pointer for which rasterizer to use.
	void(BERenderProgrammablePipeline::* pRasterizerFunc)(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);

	// method pointer for which pixel shader to use.
	void(BERenderProgrammablePipeline::* pPixelShaderFunc)(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput);

	std::wstring GetStats();

	bool backFaceCull = true;
	float backFaceAttenuation = 0.333f;

protected:
	BEScene* pScene = nullptr;
	BECamera* pCamera = nullptr;
	BECanvas* pCanvas = nullptr;

	BESurface2D<BEPipelinePSData> pixelShaderBuffer;
	BESurface2D<float> depthBuffer;

	BEPipelineVSData* vsBuffer = nullptr;
	unsigned int vsBufferSize = 10000; // To do: what size is sensible? Handle resizing?

	void DrawPoint(BEPipelineVSConstants& constants, BEPipelineVSData* pVS);
	void DrawLine(BEPipelineVSConstants& constants, BEPipelineVSData* pFrom, BEPipelineVSData* pTo);

	// draw triangles... left to right
	inline void DrawHorizontalLineLR(BEPipelineVSConstants& constants, BEPipelineVSData* pFromLeft, BEPipelineVSData* pToRight);
	void DrawTriangleFlatTopLR(BEPipelineVSConstants& constants, BEPipelineVSData* pBottom, BEPipelineVSData* pTopLeft, BEPipelineVSData* pTopRight);
	void DrawTriangleFlatBottomLR(BEPipelineVSConstants& constants, BEPipelineVSData* pBottomLeft, BEPipelineVSData* pBottomRight, BEPipelineVSData* pTop);

	inline DirectX::XMVECTOR ScreenSpaceToPixelCoord(DirectX::XMVECTOR v);

	inline bool CheckAndSetDepthBuffer(unsigned int x, unsigned int y, float depth);

	float width;
	float height;

	DirectX::XMVECTOR xmv1100 = { 1.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR halfWidthHeight11;

	DirectX::XMVECTOR backFaceOffset = { 0.0f, 0.0f, 0.001f, 0.0f };

	// stats
	long drawTime = 0;
	long vertexTime = 0;
	long geometryTime = 0;
	long pixelTime = 0;
	long clearTime = 0;
	long frameCount = 0;
	float GetAvgDrawMS() { return (float)drawTime / (float)frameCount; }
	float GetAvgVertexMS() { return (float)vertexTime / (float)frameCount; }
	float GetAvgGeomteryMS() { return (float)geometryTime / (float)frameCount; }
	float GetAvgPixelMS() { return (float)pixelTime / (float)frameCount; }
	float GetAvgClearMS() { return (float)clearTime / (float)frameCount; }

	void ClearFrameStats();
	long vertexCount = 0;
	long triangleCount = 0;
	long cullCount = 0;
	long drawCount = 0;
	long pixelCount = 0;
};
