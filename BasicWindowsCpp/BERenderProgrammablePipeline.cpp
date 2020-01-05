#include "BERenderProgrammablePipeline.h"

using namespace DirectX;

BERenderProgrammablePipeline::BERenderProgrammablePipeline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas) :
	pixelShaderBuffer(_pCanvas->width, _pCanvas->height),
	depthBuffer(_pCanvas->width, _pCanvas->height)
{
	pScene  = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;

	vsBuffer = new BEPipelineVSData[vsBufferSize];

	assert(vsBuffer != nullptr);
}

BERenderProgrammablePipeline::~BERenderProgrammablePipeline()
{
	if (vsBuffer) delete vsBuffer;
}

void BERenderProgrammablePipeline::Draw()
{
	// assemble
	//   vertex shading
	//     vertex shader
	//   geometry shader
	//     rasteriser
	// pixelshading
	//   pixel shader

	Clear();

	for (BEModel* pModel : pScene->models)
	{
		for (BEEntity* pEntity : pModel->entities)
		{
			VertexShading(pModel, pEntity);
			GeometryShader(pModel, pEntity);
		}
	}

	PixelShading();
}

void BERenderProgrammablePipeline::Clear()
{
	pixelShaderBuffer.Clear();
	depthBuffer.Clear();
}

void BERenderProgrammablePipeline::VertexShading(BEModel* pModel, BEEntity* pEntity)
{
	unsigned int indx = 0;
	BEMesh* pMesh = pModel->pMesh;
	BEVertex* pVertex = pMesh->verticies;
	BEPipelineVSData* pVSData = vsBuffer;

	assert(vsBufferSize > pMesh->vertCount); // to do: handle resize?

	while (indx < pMesh->vertCount)
	{
		VertexShader(pEntity, pVertex, pVSData);

		pVertex++;
		pVSData++;
		indx++;
	}
}

void BERenderProgrammablePipeline::VertexShader(BEEntity* pEntity, BEVertex* pVertex, BEPipelineVSData* pOutput)
{
	pOutput->positionWS = pEntity->ModelToWorldPosition(pVertex->position);
	pOutput->normalWS = pEntity->ModelToWorldDirection(pVertex->normal);
	pOutput->positionSS = pCamera->WorldToScreen(pOutput->positionWS);
	pOutput->color = pVertex->color;
	pOutput->texcoord = pVertex->texcoord;
}

void BERenderProgrammablePipeline::GeometryShader(BEModel* pModel, BEEntity* pEntity)
{
	unsigned int indx = 0;
	BEMesh* pMesh = pModel->pMesh;

	BEPipelineVSData* pVSData = vsBuffer;
	
	if (pMesh->topology == BEMesh::BEMeshTopology::TRIANGLE_LIST)
	{
		while (indx < pMesh->vertCount)
		{
			RasterizerPoints(pVSData++, pVSData++, pVSData++, pModel, pEntity);
			indx += 3;
		}
	}
	else if (pMesh->topology == BEMesh::BEMeshTopology::TRIANGLE_INDEX)
	{
		unsigned int* pIndx = pMesh->indicies;

		while (indx < pMesh->indxCount)
		{
			RasterizerPoints(pVSData + *(pIndx++), pVSData + *(pIndx++), pVSData + *(pIndx++), pModel, pEntity);
			indx += 3;
		}
	}
}

bool BERenderProgrammablePipeline::Cull(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	if (pCamera->OveralpsScreen(pv0->positionSS) ||
		pCamera->OveralpsScreen(pv1->positionSS) ||
		pCamera->OveralpsScreen(pv2->positionSS)
		) // at least 1 point on the screen
	{
		if (pCamera->IsVisible(pv0->positionWS, pv0->normalWS)) // is facing the camera
		{
			return false;
		}
	}

	return true;
}

void BERenderProgrammablePipeline::RasterizerPoints(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity)
{
	if (!pCamera->IsVisible(pv0->positionWS, pv0->normalWS)) return;

	if (pCamera->OveralpsScreen(pv0->positionSS))
	{
		int x, y;
		pixelShaderBuffer.GetIndexFromRelativePosition(pv0->positionSS.m128_f32[0], pv0->positionSS.m128_f32[1], x, y);
		float z = pv0->positionSS.m128_f32[2];

		BEPipelinePSData& psData = pixelShaderBuffer.GetDataByRef(x, y);
		psData = *pv0;
		psData.pModel = pModel;
		psData.pEntity = pEntity;
	}
	if (pCamera->OveralpsScreen(pv1->positionSS))
	{
		int x, y;
		pixelShaderBuffer.GetIndexFromRelativePosition(pv1->positionSS.m128_f32[0], pv1->positionSS.m128_f32[1], x, y);
		float z = pv1->positionSS.m128_f32[2];

		BEPipelinePSData& psData = pixelShaderBuffer.GetDataByRef(x, y);
		psData = *pv1;
		psData.pModel = pModel;
		psData.pEntity = pEntity;
	}
	if (pCamera->OveralpsScreen(pv2->positionSS))
	{
		int x, y;
		pixelShaderBuffer.GetIndexFromRelativePosition(pv2->positionSS.m128_f32[0], pv2->positionSS.m128_f32[1], x, y);
		float z = pv2->positionSS.m128_f32[2];

		BEPipelinePSData& psData = pixelShaderBuffer.GetDataByRef(x, y);
		psData = *pv2;
		psData.pModel = pModel;
		psData.pEntity = pEntity;
	}
}

#define BERENDER_ORDER_ON_Y(pv0, pv1, pv2) \
	if (pv0->positionSS.m128_f32[1] > pv1->positionSS.m128_f32[1]) std::swap(pv0, pv1); \
	if (pv1->positionSS.m128_f32[1] > pv2->positionSS.m128_f32[1]) { \
		std::swap(pv1, pv2); \
		if (pv0->positionSS.m128_f32[1] > pv1->positionSS.m128_f32[1]) std::swap(pv0, pv1); } \

void BERenderProgrammablePipeline::RasterizerWireframe(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity)
{
	if (!pCamera->OveralpsScreen(pv0->positionSS) && !pCamera->OveralpsScreen(pv1->positionSS) && !pCamera->OveralpsScreen(pv2->positionSS)) return;

	bool backface = !pCamera->IsVisible(pv0->positionWS, pv0->normalWS);

	BERENDER_ORDER_ON_Y(pv0, pv1, pv2);

	//{
	//	int x, y;
	//	pixelShaderBuffer.GetIndexFromRelativePosition(pv0->positionSS.m128_f32[0], pv0->positionSS.m128_f32[1], x, y);
	//	float z = pv0->positionSS.m128_f32[2];

	//	BEPipelinePSData& psData = pixelShaderBuffer.GetDataByRef(x, y);
	//	psData = *pv0;
	//	psData.pModel = pModel;
	//	psData.pEntity = pEntity;
	//}
}

void BERenderProgrammablePipeline::PixelShading()
{
	for (unsigned int y = 0; y < pCanvas->height; y++)
	{
		for (unsigned int x = 0; x < pCanvas->width; x++)
		{
			XMVECTOR color; //to do: later write direct to buffer
			PixelShader(pixelShaderBuffer.GetData(x, y), &color);
			pCanvas->bufferSurface->SetValue(x, y, color);
		}
	}
}

void BERenderProgrammablePipeline::PixelShader(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput)
{
	*pOutput = pPSData->color;
}
