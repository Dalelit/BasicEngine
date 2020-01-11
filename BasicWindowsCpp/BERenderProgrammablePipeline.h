#pragma once
#include "BERenderPipeline.h"


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

class BERenderProgrammablePipeline : public BERenderPipeline
{
public:
	BERenderProgrammablePipeline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderProgrammablePipeline();

	void Draw();
	void Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height) { throw "Not implemented yet"; }

	void Clear();

	void VertexShading(BEModel* pModel, BEEntity* pEntity);
	void VertexShader(BEEntity* pEntity, BEVertex* pVertex, BEPipelineVSData* pOutput);
	void GeometryShader(BEModel* pModel, BEEntity* pEntity);
	void RasterizerPoints(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity);
	void RasterizerWireframe(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity);
	void RasterizerTriangle(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity);
	void PixelShading();
	void PixelShader(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput);

	float GetAvgDrawMS() { return (float)drawTime / (float)frameCount; }
	float GetAvgVertexMS() { return (float)vertexTime / (float)frameCount; }
	float GetAvgGeomteryMS() { return (float)geometryTime / (float)frameCount; }
	float GetAvgPixelMS() { return (float)pixelTime / (float)frameCount; }
	float GetAvgClearMS() { return (float)clearTime / (float)frameCount; }

protected:
	BESurface2D<BEPipelinePSData> pixelShaderBuffer;
	BESurface2D<float> depthBuffer;

	BEPipelineVSData* vsBuffer = nullptr;
	unsigned int vsBufferSize = 10000; // To do: what size is sensible? Handle resizing?

	void DrawPoint(BEPipelineVSData* pVS, BEModel* pModel, BEEntity* pEntity, bool backFace);
	void DrawLine(BEPipelineVSData* pFrom, BEPipelineVSData* pTo, BEModel* pModel, BEEntity* pEntity, bool backFace);

	// draw triangles... left to right
	inline void DrawHorizontalLineLR(BEPipelineVSData* pFromLeft, BEPipelineVSData* pToRight, BEModel* pModel, BEEntity* pEntity);
	void DrawTriangleFlatTopLR(BEPipelineVSData* pBottom, BEPipelineVSData* pTopLeft, BEPipelineVSData* pTopRight, BEModel* pModel, BEEntity* pEntity);
	void DrawTriangleFlatBottomLR(BEPipelineVSData* pBottomLeft, BEPipelineVSData* pBottomRight, BEPipelineVSData* pTop, BEModel* pModel, BEEntity* pEntity);

	inline DirectX::XMVECTOR ScreenSpaceToPixelCoord(DirectX::XMVECTOR v);

	inline bool IsOnCanvas(DirectX::XMVECTOR& v);

	inline bool CheckAndSetDepthBuffer(unsigned int x, unsigned int y, float depth);

	float width;
	float height;

	DirectX::XMVECTOR xmv1100 = { 1.0f, 1.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR halfWidthHeight11;

	float backFaceAttenuation = 0.3f;
	DirectX::XMVECTOR backFaceOffset = { 0.0f, 0.0f, 0.001f, 0.0f };

	long drawTime = 0;
	long vertexTime = 0;
	long geometryTime = 0;
	long pixelTime = 0;
	long clearTime = 0;
	long frameCount = 0;
};
