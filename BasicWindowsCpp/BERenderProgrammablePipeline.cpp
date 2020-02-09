#include "BERenderProgrammablePipeline.h"
#include "BERaytrace.h"
#include <time.h>
#include <execution>

// To Do:
// - check if the normalising of normals is being done too much.

using namespace DirectX;

#define GETX(v) XMVectorGetX(v)
#define GETY(v) XMVectorGetY(v)
#define GETZ(v) XMVectorGetZ(v)
#define GETW(v) XMVectorGetW(v)
#define ROUND_TO_INT(f) (int)ceilf(f - 0.5f)
#define ROUND_TO_INT_X(v) (int)ceilf(GETX(v) - 0.5f)
#define ROUND_TO_INT_Y(v) (int)ceilf(GETY(v) - 0.5f)
#define ROUND_TO_INT_Z(v) (int)ceilf(GETZ(v) - 0.5f)

inline BEPipelineVSData BEPipelineVSData::operator-(const BEPipelineVSData& rhs)
{
	return {
		positionWS - rhs.positionWS,
		normalWS - rhs.normalWS,
		positionSS - rhs.positionSS,
		color - rhs.color,
		{texcoord.x - rhs.texcoord.x, texcoord.y - rhs.texcoord.y}
	};
}

inline BEPipelineVSData BEPipelineVSData::operator+(const BEPipelineVSData& rhs)
{
	return {
		positionWS + rhs.positionWS,
		normalWS + rhs.normalWS,
		positionSS + rhs.positionSS,
		color + rhs.color,
		{texcoord.x + rhs.texcoord.x, texcoord.y + rhs.texcoord.y}
	};
}

inline BEPipelineVSData BEPipelineVSData::operator*(const float rhs)
{
	return {
		positionWS * rhs,
		normalWS * rhs,
		positionSS * rhs,
		color * rhs,
		{texcoord.x * rhs, texcoord.y * rhs}
	};
}

inline BEPipelineVSData BEPipelineVSData::operator/(const float rhs)
{
	return {
		positionWS / rhs,
		normalWS / rhs,
		positionSS / rhs,
		color / rhs,
		{texcoord.x / rhs, texcoord.y / rhs}
	};
}

inline void BEPipelineVSData::operator+=(const BEPipelineVSData& rhs)
{
	positionWS += rhs.positionWS;
	normalWS += rhs.normalWS;
	positionSS += rhs.positionSS;
	color += rhs.color;
	texcoord.x += rhs.texcoord.x;
	texcoord.y += rhs.texcoord.y;
}

inline void BEPipelineVSData::operator*=(const float rhs)
{
	positionWS *= rhs;
	normalWS *= rhs;
	positionSS *= rhs;
	color *= rhs;
	texcoord.x *= rhs;
	texcoord.y *= rhs;
}

inline void BEPipelineVSData::operator/=(const float rhs)
{
	positionWS /= rhs;
	normalWS /= rhs;
	positionSS /= rhs;
	color /= rhs;
	texcoord.x /= rhs;
	texcoord.y /= rhs;
}

BERenderProgrammablePipeline::BERenderProgrammablePipeline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas) :
	pixelShaderBuffer(_pCanvas->Width(), _pCanvas->Height()),
	depthBuffer(_pCanvas->Width(), _pCanvas->Height()),
	pointerBuffer(_pCanvas->Width(), _pCanvas->Height())
{
	pScene  = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;

	width = (float)pCanvas->Width();
	height = (float)pCanvas->Height();

	halfWidthHeight11 = { width / 2.0f, height / 2.0f, 1.0f, 1.0f };

	// preallocated memory for vertex data output
	vsBuffer = new BEPipelineVSData[vsBufferSize];

	assert(vsBuffer != nullptr);

	// default functions set
	pRasterizerFunc = &BERenderProgrammablePipeline::RasterizerTriangle;
	pPixelShaderFunc = &BERenderProgrammablePipeline::PixelShaderFull;

	// initialise some common pointers in the buffer. Need for parallel for_each in pixel shader
	BEPipelinePointers ptrs;
	ptrs.depth = depthBuffer.Begin();
	ptrs.source = pixelShaderBuffer.Begin();
	ptrs.target = pCanvas->bmpSurface->Begin();

	BEPipelinePointers* pCurrent = pointerBuffer.Begin();
	BEPipelinePointers* pEnd = pointerBuffer.End();
	for (; pCurrent != pEnd; pCurrent++) { *pCurrent = ptrs; ptrs.inc(); }
}

BERenderProgrammablePipeline::~BERenderProgrammablePipeline()
{
	if (vsBuffer) delete vsBuffer;
}

void BERenderProgrammablePipeline::Draw()
{
	// initialise
	// assemble
	//   vertex shading
	//     vertex shader
	//   geometry shader
	//     rasteriser
	// pixelshading
	//   pixel shader

	clock_t startTime = clock();

	ClearFrameStats();

	Clear();

	BEPipelineVSConstants vsConstants;

	for (BEModel* pModel : pScene->models)
	{
		vsConstants.pModel = pModel;

		for (BEEntity& entity : pModel->entities)
		{
			vsConstants.pEntity = &entity;

			VertexShading(vsConstants);
			GeometryShader(vsConstants);
		}
	}

	PixelShading();

	drawTime += clock() - startTime;
	frameCount++;
}

void BERenderProgrammablePipeline::Clear()
{
	clock_t startTime = clock();

	pCanvas->Clear();
	//pixelShaderBuffer.Clear(); // too slow and not needed
	depthBuffer.Clear(depthDefaultValue);

	clearTime += clock() - startTime;
}

void BERenderProgrammablePipeline::VertexShading(BEPipelineVSConstants& constants)
{
	clock_t startTime = clock();

	unsigned int indx = 0;
	BEMesh* pMesh = constants.pModel->pMesh;
	BEVertex* pVertex = pMesh->verticies;
	BEPipelineVSData* pVSData = vsBuffer;

	assert(vsBufferSize > pMesh->vertCount); // to do: handle resize?

	while (indx < pMesh->vertCount)
	{
		VertexShader(constants, pVertex, pVSData);

		pVertex++;
		pVSData++;
		indx++;
	}

	vertexTime += clock() - startTime;
}

void BERenderProgrammablePipeline::VertexShader(BEPipelineVSConstants& constants, BEVertex* pVertex, BEPipelineVSData* pOutput)
{
	vertexCount++;

	pOutput->positionWS = constants.pEntity->ModelToWorldPosition(pVertex->position);
	pOutput->normalWS = XMVector3Normalize(constants.pEntity->ModelToWorldDirection(pVertex->normal));
	pOutput->positionSS = pCamera->WorldToScreen(pOutput->positionWS);
	pOutput->color = pVertex->color;
	pOutput->texcoord = pVertex->texcoord;
}

void BERenderProgrammablePipeline::GeometryShader(BEPipelineVSConstants& constants)
{
	clock_t startTime = clock();

	unsigned int indx = 0;
	BEMesh* pMesh = constants.pModel->pMesh;

	BEPipelineVSData* pVSData = vsBuffer;
	
	if (pMesh->topology == BEMesh::BEMeshTopology::TRIANGLE_LIST)
	{
		while (indx < pMesh->vertCount)
		{
			BEPipelineVSData* pv0 = pVSData++;
			BEPipelineVSData* pv1 = pVSData++;
			BEPipelineVSData* pv2 = pVSData++;

			CullClipDraw(constants, pv0, pv1, pv2);

			indx += 3;
		}
	}
	else if (pMesh->topology == BEMesh::BEMeshTopology::TRIANGLE_INDEX)
	{
		unsigned int* pIndx = pMesh->indicies;

		while (indx < pMesh->indxCount)
		{
			BEPipelineVSData* pv0 = &pVSData[*(pIndx++)];
			BEPipelineVSData* pv1 = &pVSData[*(pIndx++)];
			BEPipelineVSData* pv2 = &pVSData[*(pIndx++)];

			CullClipDraw(constants, pv0, pv1, pv2);

			indx += 3;
		}
	}

	geometryTime += clock() - startTime;
}

void BERenderProgrammablePipeline::CullClipDraw(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	triangleCount++;

	// Backface culling
	bool backFace = false;
	if (!pCamera->IsVisible(pv0->positionWS, pv0->normalWS) && !pCamera->IsVisible(pv1->positionWS, pv1->normalWS) && !pCamera->IsVisible(pv2->positionWS, pv2->normalWS))
		backFace = true;

	if (backFaceCull && backFace)
	{
		cullCount++;
		return;
	}

	// View space culling

	float w0 = GETW(pv0->positionSS);
	float w1 = GETW(pv1->positionSS);
	float w2 = GETW(pv2->positionSS);

	if ((GETX(pv0->positionSS) < -w0 && GETX(pv1->positionSS) < -w1 && GETX(pv2->positionSS) < -w2) ||
		(GETX(pv0->positionSS) > w0&& GETX(pv1->positionSS) > w1&& GETX(pv2->positionSS) > w2) ||
		(GETY(pv0->positionSS) < -w0 && GETY(pv1->positionSS) < -w1 && GETY(pv2->positionSS) < -w2) ||
		(GETY(pv0->positionSS) > w0&& GETY(pv1->positionSS) > w1&& GETY(pv2->positionSS) > w2) ||
		(GETZ(pv0->positionSS) < 0.0f && GETZ(pv1->positionSS) < 0.0f && GETZ(pv2->positionSS) < 0.0f) ||
		(GETZ(pv0->positionSS) > w0 && GETZ(pv1->positionSS) > w1 && GETZ(pv2->positionSS) > w2))
	{
		cullCount++;
		return;
	}

	// Clipping - near (<0) and far (>w) plane
	// else draw
	if (GETZ(pv0->positionSS) < 0.0f)
	{
		if (GETZ(pv1->positionSS) < 0.0f) Clip12DrawNear(constants, pv2, pv0, pv1, backFace); // v0 & v1 are out
		else if (GETZ(pv2->positionSS) < 0.0f) Clip12DrawNear(constants, pv1, pv0, pv2, backFace); // v0 & v2 are out
		else Clip0DrawNear(constants, pv0, pv1, pv2, backFace); // v0 only out
	}
	else if (GETZ(pv1->positionSS) < 0.0f)
	{
		if (GETZ(pv2->positionSS) < 0.0f) Clip12DrawNear(constants, pv0, pv1, pv2, backFace); // v1 & v2 are out
		else Clip0DrawNear(constants, pv1, pv0, pv2, backFace); // v1 only out
	}
	else if (GETZ(pv2->positionSS) < 0.0f)
	{
		Clip0DrawNear(constants, pv2, pv0, pv1, backFace); // v2 only out
	}
	else if (GETZ(pv0->positionSS) > w0)
	{
		if (GETZ(pv1->positionSS) > w1) Clip12DrawFar(constants, pv2, pv0, pv1, backFace); // v0 & v1 are out
		else if (GETZ(pv2->positionSS) > w2) Clip12DrawFar(constants, pv1, pv0, pv2, backFace); // v0 & v2 are out
		else Clip0DrawFar(constants, pv0, pv1, pv2, backFace); // v0 only out
	}
	else if (GETZ(pv1->positionSS) > w1)
	{
		if (GETZ(pv2->positionSS) > w2) Clip12DrawFar(constants, pv0, pv1, pv2, backFace); // v1 & v2 are out
		else Clip0DrawFar(constants, pv1, pv0, pv2, backFace); // v1 only out
	}
	else if (GETZ(pv2->positionSS) > w2)
	{
		Clip0DrawFar(constants, pv2, pv0, pv1, backFace); // v2 only out
	}
	else Draw(constants, pv0, pv1, pv2, backFace); // none need clipping
}

void BERenderProgrammablePipeline::Clip0DrawNear(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace)
{
	float z0 = GETZ(pv0->positionSS);
	float z1 = GETZ(pv1->positionSS);
	float z2 = GETZ(pv2->positionSS);
	float a1 = (-z0) / (z1 - z0);
	float a2 = (-z0) / (z2 - z0);

	BEPipelineVSData v01 = (*pv0) + ((*pv1) - (*pv0)) * a1;
	BEPipelineVSData v02 = (*pv0) + ((*pv2) - (*pv0)) * a2;

	clipCount++;
	Draw(constants, &v01, pv1, pv2, backFace);
	Draw(constants, &v02, &v01, pv2, backFace);
}

void BERenderProgrammablePipeline::Clip12DrawNear(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace)
{
	float z0 = GETZ(pv0->positionSS);
	float z1 = GETZ(pv1->positionSS);
	float z2 = GETZ(pv2->positionSS);
	float a1 = (-z0) / (z1 - z0);
	float a2 = (-z0) / (z2 - z0);

	BEPipelineVSData v01 = (*pv0) + ((*pv1) - (*pv0)) * a1;
	BEPipelineVSData v02 = (*pv0) + ((*pv2) - (*pv0)) * a2;

	clipCount++;
	Draw(constants, pv0, &v01, &v02, backFace);
}

void BERenderProgrammablePipeline::Clip0DrawFar(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace)
{
	float z0 = GETZ(pv0->positionSS) / GETW(pv0->positionSS);
	float z1 = GETZ(pv1->positionSS) / GETW(pv1->positionSS);
	float z2 = GETZ(pv2->positionSS) / GETW(pv2->positionSS);
	float a1 = (1.0f - z0) / (z1 - z0);
	float a2 = (1.0f - z0) / (z2 - z0);

	BEPipelineVSData v01 = (*pv0) + ((*pv1) - (*pv0)) * a1;
	BEPipelineVSData v02 = (*pv0) + ((*pv2) - (*pv0)) * a2;

	clipCount++;
	Draw(constants, &v01, pv1, pv2, backFace);
	Draw(constants, &v02, &v01, pv2, backFace);
}

void BERenderProgrammablePipeline::Clip12DrawFar(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace)
{
	float z0 = GETZ(pv0->positionSS) / GETW(pv0->positionSS);
	float z1 = GETZ(pv1->positionSS) / GETW(pv1->positionSS);
	float z2 = GETZ(pv2->positionSS) / GETW(pv2->positionSS);
	float a1 = (1.0f - z0) / (z1 - z0);
	float a2 = (1.0f - z0) / (z2 - z0);

	BEPipelineVSData v01 = (*pv0) + ((*pv1) - (*pv0)) * a1;
	BEPipelineVSData v02 = (*pv0) + ((*pv2) - (*pv0)) * a2;

	clipCount++;
	Draw(constants, pv0, &v01, &v02, backFace);
}

void BERenderProgrammablePipeline::Draw(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, bool backFace)
{
	// copy so the vertex doesn't get updated.
	BEPipelineVSData v0(*pv0);
	BEPipelineVSData v1(*pv1);
	BEPipelineVSData v2(*pv2);

	// set screen space to actual coordinates
	v0.positionSS = ScreenSpaceToPixelCoord(pv0->positionSS / GETW(pv0->positionSS));
	v1.positionSS = ScreenSpaceToPixelCoord(pv1->positionSS / GETW(pv1->positionSS));
	v2.positionSS = ScreenSpaceToPixelCoord(pv2->positionSS / GETW(pv2->positionSS));

	if (backFace) // for when back face is drawn
	{
		v0.color *= backFaceAttenuation;
		v1.color *= backFaceAttenuation;
		v2.color *= backFaceAttenuation;
		v0.positionSS += backFaceOffset;
		v1.positionSS += backFaceOffset;
		v2.positionSS += backFaceOffset;
	}

	// Draw using the specified rasteriser
	drawCount++;
	(this->*pRasterizerFunc)(constants, &v0, &v1, &v2);
}

void BERenderProgrammablePipeline::RasterizerPoints(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	DrawPoint(constants, pv0);
	DrawPoint(constants, pv1);
	DrawPoint(constants, pv2);
}

void BERenderProgrammablePipeline::RasterizerWireframe(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	DrawLine(constants, pv0, pv1);
	DrawLine(constants, pv0, pv2);
	DrawLine(constants, pv1, pv2);
}

void BERenderProgrammablePipeline::RasterizerTriangle(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	// sort verticies, lowest y to highest.
	if (GETY(pv0->positionSS) > GETY(pv1->positionSS)) std::swap(pv0, pv1);
	if (GETY(pv1->positionSS) > GETY(pv2->positionSS)) {
		std::swap(pv1, pv2);
		if (GETY(pv0->positionSS) > GETY(pv1->positionSS)) std::swap(pv0, pv1);
	}

	float y0 = GETY(pv0->positionSS);
	float y1 = GETY(pv1->positionSS);
	float y2 = GETY(pv2->positionSS);

	float x0 = GETX(pv0->positionSS);
	float x1 = GETX(pv1->positionSS);
	float x2 = GETX(pv2->positionSS);

	// draw the triangles

	if (y0 == y1) // flat bottom triangle only
	{
		if (x0 > x1) std::swap(pv0, pv1); // ensure left to right

		DrawTriangleFlatBottomLR(constants, pv0, pv1, pv2);
	}
	else if (y1 == y2) // flat top triangle only
	{
		if (x1 > x2) std::swap(pv1, pv2); // ensure left to right

		DrawTriangleFlatTopLR(constants, pv0, pv1, pv2);
	}
	else // create the 2 flat top/bottom triangles
	{
		float split = (y1 - y0) / (y2 - y0); // or slope
		BEPipelineVSData v1a(*pv0 + (*pv2 - *pv0) * split); // from the start + the difference * split
		BEPipelineVSData* pv1a = &v1a; // need a pointer to it for swapping

		if (x1 > GETX(pv1a->positionSS)) std::swap(pv1, pv1a); // ensure left to right

		DrawTriangleFlatTopLR(constants, pv0, pv1, pv1a);
		DrawTriangleFlatBottomLR(constants, pv1, pv1a, pv2);
	}
}

void BERenderProgrammablePipeline::RasterizerWireFrameAndNormals(BEPipelineVSConstants& constants, BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	DrawLine(constants, pv0, pv1);
	DrawLine(constants, pv0, pv2);
	DrawLine(constants, pv1, pv2);

	auto SetNormalPoint = [this](BEPipelineVSData& n, BEPipelineVSData* pv) {
		n = *pv;
		n.positionWS += n.normalWS * 0.25;
		n.positionWS.m128_f32[3] = 1.0f;
		n.positionSS = pCamera->WorldToScreen(n.positionWS);
		n.positionSS = ScreenSpaceToPixelCoord(n.positionSS / GETW(n.positionSS));
		n.color = {1,0,0,1};
	};

	BEPipelineVSData normalv;

	SetNormalPoint(normalv, pv0);
	DrawLine(constants, pv0, &normalv);

	SetNormalPoint(normalv, pv1);
	DrawLine(constants, pv1, &normalv);

	SetNormalPoint(normalv, pv2);
	DrawLine(constants, pv2, &normalv);
}

void BERenderProgrammablePipeline::PixelShading()
{
	clock_t startTime = clock();

	auto pixelFunc = [this](BEPipelinePointers& ptr) {
		if (*ptr.depth < depthDefaultValue)
		{
			*ptr.target = (this->*pPixelShaderFunc)(ptr.source) * 255.0f;
			pixelCount++;
		}
	};

	std::for_each(std::execution::par, pointerBuffer.Begin(), pointerBuffer.End(), pixelFunc);

	pixelTime += clock() - startTime;
}

DirectX::XMVECTOR BERenderProgrammablePipeline::PixelShaderFull(BEPipelinePSData* pPSData)
{
	BEMaterial* pMat = pPSData->pEntity->pMaterial;

	XMVECTOR texColor = g_XMZero;

	if (pMat->IsTextured())
	{
		texColor = pMat->pTextureSampler->SampleClosest(pPSData->texcoord);
	}

	XMVECTOR lights = pScene->ambientLight.CalculateColor(pMat);
	lights += pScene->directionalLight.CalculateColorInWorldSpace(pPSData->normalWS, pMat, texColor);

	for (auto l : pScene->lights)
	{
		lights += l->CalculateColorSpecInWorldSpace(pPSData->positionWS, pPSData->normalWS, pCamera->position, pMat, texColor);
	}

	return XMVectorSaturate(lights);
}

DirectX::XMVECTOR BERenderProgrammablePipeline::PixelShaderColorLight(BEPipelinePSData* pPSData)
{
	BEMaterial* pMat = pPSData->pEntity->pMaterial;

	XMVECTOR lights = pScene->ambientLight.CalculateColor(pMat);
	lights += pScene->directionalLight.CalculateColorInWorldSpace(pPSData->normalWS, pMat);

	return XMVectorSaturate(lights);
}

DirectX::XMVECTOR BERenderProgrammablePipeline::PixelShaderColor(BEPipelinePSData* pPSData)
{
	return pPSData->color;
}

DirectX::XMVECTOR BERenderProgrammablePipeline::PixelShaderPointOnly(BEPipelinePSData* pPSData)
{
	BEMaterial* pMat = pPSData->pEntity->pMaterial;

	XMVECTOR lights = {};

	for (auto l : pScene->lights)
	{
		lights += l->CalculateColorSpecInWorldSpace(pPSData->positionWS, pPSData->normalWS, pCamera->position, pMat);
	}

	return XMVectorSaturate(pPSData->color * lights);
}

void BERenderProgrammablePipeline::DrawPoint(BEPipelineVSConstants& constants, BEPipelineVSData* pVS)
{
	int x = ROUND_TO_INT_X(pVS->positionSS);
	if (x < 0 || x >= (int)width) return;

	int y = ROUND_TO_INT_Y(pVS->positionSS);
	if (y < 0 || y >= (int)height) return;

	float z = GETZ(pVS->positionSS);

	if (CheckAndSetDepthBuffer(x, y, z))
	{
		BEPipelinePSData& psData = pixelShaderBuffer.GetDataByRef(x, y);
		psData = (*pVS);
		psData.pModel = constants.pModel;
		psData.pEntity = constants.pEntity;
	}
}

void BERenderProgrammablePipeline::DrawLine(BEPipelineVSConstants& constants, BEPipelineVSData* pFrom, BEPipelineVSData* pTo)
{
	float fromXf = GETX(pFrom->positionSS);
	float fromYf = GETY(pFrom->positionSS);
	float toXf = GETX(pTo->positionSS);
	float toYf = GETY(pTo->positionSS);
	float dx = toXf - fromXf;
	float dy = toYf - fromYf;

	if (fabsf(dx) > fabsf(dy)) // x distance longer than y
	{
		if (fromXf > toXf) // always draw left to right
		{
			std::swap(pFrom, pTo);
			std::swap(fromXf, toXf);
			std::swap(fromYf, toYf);
			dx = -dx;
		}

		int fromX = ROUND_TO_INT(fromXf);
		int fromY = ROUND_TO_INT(fromYf);
		int toX = ROUND_TO_INT(toXf);

		if (toX < 0) return;			 // for when the whole line is left of the screen
		if (fromX >= (int)width) return; // for when the while line is left of the screen

		if (fromX < 0) fromX = 0;				// if starting left of the screen, start on 0;
		if (toX > (int)width) toX = (int)width; // if going past the screen, stop at the end

		BEPipelineVSData from = *pFrom;
		BEPipelineVSData delta = (*pTo - from) / dx;

		from = from + delta * (((float)fromX + 0.5f) - fromXf); // move to the first pixel

		while (fromX < toX)
		{
			fromY = ROUND_TO_INT_Y(from.positionSS);

			if (fromY >= 0 && fromY < (int)height && CheckAndSetDepthBuffer(fromX, fromY, GETZ(from.positionSS)))
			{
				auto pPSData = pixelShaderBuffer.GetData(fromX, fromY);
				*pPSData = from;
				pPSData->pModel = constants.pModel;
				pPSData->pEntity = constants.pEntity;
			}

			from += delta;
			fromX++;
		}
	}
	else  // y distance longer than x
	{
		if (fromYf > toYf) // always draw lower y up.
		{
			std::swap(pFrom, pTo);
			std::swap(fromXf, toXf);
			std::swap(fromYf, toYf);
			dy = -dy;
		}

		int fromX = ROUND_TO_INT(fromXf);
		int fromY = ROUND_TO_INT(fromYf);
		int toY = ROUND_TO_INT(toYf);

		if (toY < 0) return;			  // for when the whole line is below the screen
		if (fromY >= (int)height) return; // for when the whole line is above the screen

		if (fromY < 0) fromY = 0;				  // if starting below the screen, start on 0;
		if (toY > (int)height) toY = (int)height; // if going past the screen, stop at the end

		BEPipelineVSData from = *pFrom;
		BEPipelineVSData delta = (*pTo - from) / dy;

		from = from + delta * (((float)fromY + 0.5f) - fromYf); // move to the first pixel

		while (fromY < toY)
		{
			fromX = ROUND_TO_INT_X(from.positionSS);

			if (fromX >= 0 && fromX < (int)width && CheckAndSetDepthBuffer(fromX, fromY, GETZ(from.positionSS)))
			{
				auto pPSData = pixelShaderBuffer.GetData(fromX, fromY);
				*pPSData = from;
				pPSData->pModel = constants.pModel;
				pPSData->pEntity = constants.pEntity;
			}

			from += delta;
			fromY++;
		}
	}
}

inline void BERenderProgrammablePipeline::DrawHorizontalLineLR(BEPipelineVSConstants& constants, BEPipelineVSData* pFromLeft, BEPipelineVSData* pToRight)
{
	// to do: may need to check it is left to right? assert

	int currentY = ROUND_TO_INT_Y(pFromLeft->positionSS); // the line

	float fromXf = GETX(pFromLeft->positionSS);
	int currentX = ROUND_TO_INT(fromXf); // starting pixel

	float toXf = GETX(pToRight->positionSS);
	int toX = ROUND_TO_INT(toXf); // end pixel to stop before

	if (currentX < 0) currentX = 0;			// if left of the screen, start at the left
	if (toX > (int)width) toX = (int)width; // if the end is past the screen, stop at the end, noting we stop before this value

	BEPipelineVSData lineDelta = (*pToRight - *pFromLeft) / (toXf - fromXf); // the change per pixel

	// optimisation - go directly to the data. Easy on a horizontal line
	auto pPixel = pixelShaderBuffer.GetData(currentX, currentY);
	auto pDepth = depthBuffer.GetData(currentX, currentY);

	float xf = (float)currentX - 0.5f; // get the centre of the location of the pixel
	BEPipelineVSData line = *pFromLeft + lineDelta * (xf - fromXf); // get the starting point

	while (currentX < toX)
	{
		line.normalWS = XMVector3Normalize(line.normalWS);
		// optimised version of
		// if (CheckAndSetDepthBuffer(currentX, currentY, GETZ(line.positionSS)))
		//   pixelShaderBuffer.SetValue(currentX, currentY, psData);
		float z = GETZ(line.positionSS);
		if (z < *pDepth)
		{
			*pDepth = z;
			*pPixel = line;
			pPixel->pModel = constants.pModel;
			pPixel->pEntity = constants.pEntity;
		}

		currentX++;
		line += lineDelta;
		pDepth++;
		pPixel++;
	}
}

void BERenderProgrammablePipeline::DrawTriangleFlatTopLR(BEPipelineVSConstants& constants, BEPipelineVSData* pBottom, BEPipelineVSData* pTopLeft, BEPipelineVSData* pTopRight)
{
	// draw flat tops down to bottom

	float toYf = GETY(pBottom->positionSS);
	float startLeftYf = GETY(pTopLeft->positionSS);
	float startRightYf = GETY(pTopRight->positionSS);

	int currentY = ROUND_TO_INT(startLeftYf) - 1; // value to start at. Going opposite direciton so start -1
	int toY = ROUND_TO_INT(toYf);			 // value to stop at. Going opposite direciton stop at, not before

	if (toY < 0) toY = 0; // if end is below the screen, change the toY to be the end
	if (currentY >= (int)height) currentY = (int)height - 1;

	BEPipelineVSData lineStartDelta = (*pBottom - *pTopLeft) / (startLeftYf - toYf);
	BEPipelineVSData lineEndDelta = (*pBottom - *pTopRight) / (startRightYf - toYf);

	float yf = (float)currentY + 0.5f; // get the centre location of the pixel
	BEPipelineVSData lineStart = *pTopLeft + lineStartDelta * (startLeftYf - yf);
	BEPipelineVSData lineEnd = *pTopRight + lineEndDelta * (startRightYf - yf);

	// draw each horizontal line
	while (currentY >= toY)
	{
		lineStart.normalWS = XMVector3Normalize(lineStart.normalWS);
		lineEnd.normalWS = XMVector3Normalize(lineEnd.normalWS);

		DrawHorizontalLineLR(constants, &lineStart, &lineEnd);

		currentY--;
		lineStart += lineStartDelta;
		lineEnd += lineEndDelta;
	}
}

void BERenderProgrammablePipeline::DrawTriangleFlatBottomLR(BEPipelineVSConstants& constants, BEPipelineVSData* pBottomLeft, BEPipelineVSData* pBottomRight, BEPipelineVSData* pTop)
{
	// draw flat bottoms up to top

	float toYf = GETY(pTop->positionSS);
	float startLeftYf = GETY(pBottomLeft->positionSS);
	float startRightYf = GETY(pBottomRight->positionSS);

	int currentY = ROUND_TO_INT(startLeftYf); // value to start at.
	int toY = ROUND_TO_INT(toYf);			  // value to stop before

	if (toY > (int)height) toY = (int)height; // end is above the screen, so stop at the top row, which is before height
	if (currentY < 0) currentY = 0;			  // starting below the screen, so start at 0

	BEPipelineVSData lineStartDelta = (*pTop - *pBottomLeft) / (toYf - startLeftYf);
	BEPipelineVSData lineEndDelta = (*pTop - *pBottomRight) / (toYf - startRightYf);

	float yf = (float)currentY + 0.5f; // get the centre location of the pixel
	BEPipelineVSData lineStart = *pBottomLeft + lineStartDelta * (yf - startLeftYf);
	BEPipelineVSData lineEnd = *pBottomRight + lineEndDelta * (yf - startRightYf);

	// draw each horizontal line
	while (currentY < toY)
	{
		lineStart.normalWS = XMVector3Normalize(lineStart.normalWS);
		lineEnd.normalWS = XMVector3Normalize(lineEnd.normalWS);

		DrawHorizontalLineLR(constants, &lineStart, &lineEnd);

		currentY++;
		lineStart += lineStartDelta;
		lineEnd += lineEndDelta;
	}
}

inline DirectX::XMVECTOR BERenderProgrammablePipeline::ScreenSpaceToPixelCoord(DirectX::XMVECTOR v)
{
	return (v + xmv1100) * halfWidthHeight11;
}

inline bool BERenderProgrammablePipeline::CheckAndSetDepthBuffer(unsigned int x, unsigned int y, float depth)
{
	auto pData = depthBuffer.GetData(x, y);
	if (depth < *pData)
	{
		*pData = depth;
		return true;
	}
	else return false;
}

std::wstring BERenderProgrammablePipeline::GetStats()
{
	std::wstringstream msg;

	msg << "Draw time: " << GetAvgDrawMS() << "ms" << std::endl;
	msg << "Clear time: " << GetAvgClearMS() << "ms" << std::endl;
	msg << "Vertex time: " << GetAvgVertexMS() << "ms" << std::endl;
	msg << "Geometry time: " << GetAvgGeomteryMS() << "ms" << std::endl;
	msg << "Pixel time: " << GetAvgPixelMS() << "ms" << std::endl;
	msg << std::endl;
	msg << "Vertex count: " << vertexCount << std::endl;
	msg << "Triangle count: " << triangleCount << std::endl;
	msg << "Triangle cull count: " << cullCount << std::endl;
	msg << "Triangle clip count: " << clipCount << std::endl;
	msg << "Triangle draw count: " << drawCount << std::endl;
	msg << "Pixel count: " << pixelCount << " of " << pixelShaderBuffer.GetSize() << std::endl;

	return msg.str();
}

void BERenderProgrammablePipeline::ClearFrameStats()
{
	vertexCount = 0;
	triangleCount = 0;
	cullCount = 0;
	clipCount = 0;
	drawCount = 0;
	pixelCount = 0;
}
