#pragma once
#include "BERenderPipeline.h"


struct BEPipelineVSData
{
	DirectX::XMVECTOR positionWS;
	DirectX::XMVECTOR normalWS;
	DirectX::XMVECTOR positionSS;
	DirectX::XMVECTOR color;
	DirectX::XMFLOAT2 texcoord;
};

struct BEPipelinePSData
{
	DirectX::XMVECTOR positionWS;
	DirectX::XMVECTOR normalWS;
	DirectX::XMVECTOR color;
	DirectX::XMFLOAT2 texcoord;
	BEModel* pModel;
	BEEntity* pEntity;

	void operator= (BEPipelineVSData& rhs) {
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
	bool Cull(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2);
	void RasterizerPoints(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity);
	void RasterizerWireframe(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity);
	void PixelShading();
	void PixelShader(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput);

protected:
	BESurface2D<BEPipelinePSData> pixelShaderBuffer;
	BESurface2D<float> depthBuffer;

	BEPipelineVSData* vsBuffer = nullptr;
	unsigned int vsBufferSize = 10000; // To do: what size is sensible? Handle resizing?
};
