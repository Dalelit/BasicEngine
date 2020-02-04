#pragma once
#include "BEScene.h"
#include "BECamera.h"
#include "BECanvas.h"
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
		//pModel(nullptr),
		//pEntity(nullptr)
		{}

	inline void operator=(BEPipelineVSData& rhs) {
		positionWS = rhs.positionWS;
		normalWS = rhs.normalWS;
		color = rhs.color;
		texcoord = rhs.texcoord;
	}
};

// note: This is static data once initialised. Seems to be quicker keeping this
//       in a separate buffer so the psData buffer is a small as possible.
struct BEPipelinePointers
{
	float* depth;
	BEPipelinePSData* source;
	BECanvas::Pixel* target;

	inline void inc() {
		depth++;
		source++;
		target++;
	}
};

class BERenderProgrammablePipeline
{
public:
	BERenderProgrammablePipeline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderProgrammablePipeline();

	void Draw();

	void Clear();

	void VertexShader(BEPipelineVSConstants& constants, BEVertex* pVertex, BEPipelineVSData* pOutput);

	void RasterizerPoints(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void RasterizerWireframe(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void RasterizerTriangle(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void RasterizerWireFrameAndNormals(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);

	void PixelShading();
	DirectX::XMVECTOR PixelShaderFull(BEPipelinePSData* pPSData);
	DirectX::XMVECTOR PixelShaderColorLight(BEPipelinePSData* pPSData);
	DirectX::XMVECTOR PixelShaderColor(BEPipelinePSData* pPSData);
	DirectX::XMVECTOR PixelShaderPointOnly(BEPipelinePSData* pPSData);

	// method pointer for which rasterizer to use.
	void(BERenderProgrammablePipeline::* pRasterizerFunc)(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);

	// method pointer for which pixel shader to use.
	DirectX::XMVECTOR (BERenderProgrammablePipeline::* pPixelShaderFunc)(BEPipelinePSData* pPSData);

	std::wstring GetStats();

	bool backFaceCull = true;
	float backFaceAttenuation = 0.333f;

protected:
	BEScene* pScene = nullptr;
	BECamera* pCamera = nullptr;
	BECanvas* pCanvas = nullptr;

	BESurface2D<BEPipelinePSData> pixelShaderBuffer;
	BESurface2D<float> depthBuffer;
	BESurface2D<BEPipelinePointers> pointerBuffer;
	static constexpr float depthDefaultValue = FLT_MAX;

	BEPipelineVSData* vsBuffer = nullptr;
	unsigned int vsBufferSize = 10000; // To do: what size is sensible? Handle resizing?

	void VertexShading(BEPipelineVSConstants& constants);
	void GeometryShader(BEPipelineVSConstants& constants);
	void CullClipDraw(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void Clip0DrawNear(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace);
	void Clip12DrawNear(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace);
	void Clip0DrawFar(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace);
	void Clip12DrawFar(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace);
	void Draw(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace);

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
	long clipCount = 0;
	long drawCount = 0;
	long pixelCount = 0;
};
