#include "BERenderProgrammablePipeline.h"
#include "BERaytrace.h"
#include <time.h>

using namespace DirectX;

#define GETX(v) XMVectorGetX(v)
#define GETY(v) XMVectorGetY(v)
#define GETZ(v) XMVectorGetZ(v)
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
	pixelShaderBuffer(_pCanvas->width, _pCanvas->height),
	depthBuffer(_pCanvas->width, _pCanvas->height)
{
	pScene  = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;

	width = (float)pCanvas->width;
	height = (float)pCanvas->height;
	widthHalf = width / 2.0f;
	heightHalf = height / 2.0f;

	vsBuffer = new BEPipelineVSData[vsBufferSize];
	//memset(vsBuffer, 0, sizeof(BEPipelineVSData) * vsBufferSize);

	assert(vsBuffer != nullptr);
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

	drawTime += clock() - startTime;
	frameCount++;
}

void BERenderProgrammablePipeline::Clear()
{
	clock_t startTime = clock();

	pixelShaderBuffer.Clear();
	depthBuffer.Clear(FLT_MAX);

	clearTime += clock() - startTime;
}

void BERenderProgrammablePipeline::VertexShading(BEModel* pModel, BEEntity* pEntity)
{
	clock_t startTime = clock();

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

	vertexTime += clock() - startTime;
}

void BERenderProgrammablePipeline::VertexShader(BEEntity* pEntity, BEVertex* pVertex, BEPipelineVSData* pOutput)
{
	pOutput->positionWS = pEntity->ModelToWorldPosition(pVertex->position);
	pOutput->normalWS = pEntity->ModelToWorldDirection(pVertex->normal);
	pOutput->positionSS = ScreenSpaceToPixelCoord(pCamera->WorldToScreen(pOutput->positionWS));
	pOutput->color = pVertex->color;
	pOutput->texcoord = pVertex->texcoord;
}

void BERenderProgrammablePipeline::GeometryShader(BEModel* pModel, BEEntity* pEntity)
{
	clock_t startTime = clock();

	unsigned int indx = 0;
	BEMesh* pMesh = pModel->pMesh;

	BEPipelineVSData* pVSData = vsBuffer;
	
	if (pMesh->topology == BEMesh::BEMeshTopology::TRIANGLE_LIST)
	{
		while (indx < pMesh->vertCount)
		{
			BEPipelineVSData* pv0 = pVSData++;
			BEPipelineVSData* pv1 = pVSData++;
			BEPipelineVSData* pv2 = pVSData++;

			//RasterizerPoints(pv0, pv1, pv2, pModel, pEntity);
			//RasterizerWireframe(pv0, pv1, pv2, pModel, pEntity);
			RasterizerTriangle(pv0, pv1, pv2, pModel, pEntity);
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

			//RasterizerPoints(pv0, pv1, pv2, pModel, pEntity);
			//RasterizerWireframe(pv0, pv1, pv2, pModel, pEntity);
			RasterizerTriangle(pv0, pv1, pv2, pModel, pEntity);
			indx += 3;
		}
	}

	geometryTime += clock() - startTime;
}

void BERenderProgrammablePipeline::RasterizerPoints(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity)
{
	bool backFace = !pCamera->IsVisible(pv0->positionWS, pv0->normalWS);

	DrawPoint(pv0, pModel, pEntity, backFace);
	DrawPoint(pv1, pModel, pEntity, backFace);
	DrawPoint(pv2, pModel, pEntity, backFace);
}

void BERenderProgrammablePipeline::RasterizerWireframe(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity)
{
	bool backFace = !pCamera->IsVisible(pv0->positionWS, pv0->normalWS);

	DrawLine(pv0, pv1, pModel, pEntity, backFace);
	DrawLine(pv0, pv2, pModel, pEntity, backFace);
	DrawLine(pv1, pv2, pModel, pEntity, backFace);
}

void BERenderProgrammablePipeline::RasterizerTriangle(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2, BEModel* pModel, BEEntity* pEntity)
{
	if (!pCamera->IsVisible(pv0->positionWS, pv0->normalWS)) return; // is not facing the camera

	if (!IsOnCanvas(pv0->positionSS) && !IsOnCanvas(pv1->positionSS) && !IsOnCanvas(pv2->positionSS)) return; // does not have at least 1 point on the screen

	// sort verticies, lowest y to highest.
	if (GETY(pv0->positionSS) > GETY(pv1->positionSS)) std::swap(pv0, pv1);
	if (GETY(pv1->positionSS) > GETY(pv2->positionSS)) {
		std::swap(pv1, pv2);
		if (GETY(pv0->positionSS) > GETY(pv1->positionSS)) std::swap(pv0, pv1);
	}

	// create the vertecies for the lower and upper triangle
	// and sort the x or correctly
	float y0 = GETY(pv0->positionSS);
	float y1 = GETY(pv1->positionSS);
	float y2 = GETY(pv2->positionSS);
	if (y0 == y1 && y0 == y2) // flat line // to do: is this a ideal? feels messy
	{
		// get leftmost vert
		BEPipelineVSData* pLeft = pv0;
		if (GETX(pLeft->positionSS) > GETX(pv1->positionSS)) pLeft = pv1;
		if (GETX(pLeft->positionSS) > GETX(pv2->positionSS)) pLeft = pv2;
		// get rightmost vert
		BEPipelineVSData* pRight = pv0;
		if (GETX(pRight->positionSS) < GETX(pv1->positionSS)) pRight = pv1;
		if (GETX(pRight->positionSS) < GETX(pv2->positionSS)) pRight = pv2;

		//DrawHorizontalLineLR(pLeft, pRight, pModel, pEntity);
	}
	else if (y0 == y1) // flat bottom triangle only
	{
		if (GETX(pv0->positionSS) > GETX(pv1->positionSS)) std::swap(pv0, pv1);
		DrawTriangleFlatBottomLR(pv0, pv1, pv2, pModel, pEntity);
	}
	else if (y1 == y2) // flat top triangle only
	{
		if (GETX(pv1->positionSS) > GETX(pv2->positionSS)) std::swap(pv1, pv2);
		DrawTriangleFlatTopLR(pv0, pv1, pv2, pModel, pEntity);
	}
	else // create the 2 flat top/bottom triangles
	{
		float y0 = GETY(pv0->positionSS);
		float y1 = GETY(pv1->positionSS);
		float y2 = GETY(pv2->positionSS);

		// new point is... start point v0 plus...
		// the delta (v2 - v0) divided by how far the interium point is (heigth of 1 / height of 2)
		float split = (y1 - y0) / (y2 - y0);
		BEPipelineVSData v1a(*pv0 + (*pv2 - *pv0) * split);
		BEPipelineVSData* pv1a = &v1a;

		if (GETX(pv1->positionSS) > GETX(pv1a->positionSS)) std::swap(pv1, pv1a);
		DrawTriangleFlatTopLR(pv0, pv1, pv1a, pModel, pEntity);
		DrawTriangleFlatBottomLR(pv1, pv1a, pv2, pModel, pEntity);
	}
}

void BERenderProgrammablePipeline::PixelShading()
{
	clock_t startTime = clock();

	//for (unsigned int y = 0; y < pCanvas->height; y++)
	//{
	//	for (unsigned int x = 0; x < pCanvas->width; x++)
	//	{
	//		XMVECTOR color;
	//		PixelShader(pixelShaderBuffer.GetData(x, y), &color);
	//		pCanvas->bufferSurface->SetValue(x, y, color);
	//	}
	//}

	auto pPSBuffer = pixelShaderBuffer.GetData();
	unsigned int size = pixelShaderBuffer.GetSize();

	auto pTarget = pCanvas->bufferSurface->GetData();

	for (unsigned int i = 0; i < size; i++)
	{
		PixelShader(pPSBuffer, pTarget);

		pPSBuffer++;
		pTarget++;
	}

	pixelTime += clock() - startTime;
}

void BERenderProgrammablePipeline::PixelShader(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput)
{
	if (!pPSData->pEntity) return; // nothing to shade

	XMVECTOR color;

	auto pMesh = pPSData->pModel->pMesh;
	if (pMesh->IsTextured())
	{
		color = pMesh->pTextureSampler->SampleClosest(pPSData->texcoord);
	}
	else
	{
		color = pPSData->color;
	}

	XMVECTOR lights = pScene->ambientLight;
	lights += pScene->directionalLight.CalculateColorInWorldSpace(pPSData->normalWS);

	*pOutput = XMVectorSaturate(color * lights);
}

void BERenderProgrammablePipeline::DrawPoint(BEPipelineVSData* pVS, BEModel* pModel, BEEntity* pEntity, bool backFace)
{
	int x = ROUND_TO_INT_X(pVS->positionSS);
	if (x < 0 || x >= (int)width) return;

	int y = ROUND_TO_INT_Y(pVS->positionSS);
	if (y < 0 || y >= (int)height) return;

	float z;
	if (backFace) z = GETZ((pVS->positionSS + backFaceOffset));
	else z = GETZ(pVS->positionSS);

	if (CheckAndSetDepthBuffer(x, y, z))
	{
		BEPipelinePSData& psData = pixelShaderBuffer.GetDataByRef(x, y);
		psData = (*pVS);
		psData.pModel = pModel;
		psData.pEntity = pEntity;

		if (backFace) psData.color *= backFaceAttenuation;
	}

}

void BERenderProgrammablePipeline::DrawLine(BEPipelineVSData* pFrom, BEPipelineVSData* pTo, BEModel* pModel, BEEntity* pEntity, bool backFace)
{
	// to do: really need to rethink this
	if (GETY(pFrom->positionSS) > GETY(pTo->positionSS)) std::swap(pFrom, pTo); // always draw lower y up.

	int toX = ROUND_TO_INT_X(pTo->positionSS);
	int toY = ROUND_TO_INT_Y(pTo->positionSS);
	int fromX = ROUND_TO_INT_X(pFrom->positionSS);
	int fromY = ROUND_TO_INT_Y(pFrom->positionSS);

	if (toY < 0) return;						// for when the whole line is below the screen
	if (fromY >= (int)height) return;			// for when the whole line is above the screen
	if (fromX < 0 && toX < 0.0f) return;		// for when the while line is left of the screen
	if (fromX >= (int)width && toX >= (int)width) return;	// for when the while line is left of the screen

	BEPipelineVSData from = *pFrom;
	BEPipelineVSData delta = *pTo - from;

	if (backFace)
	{
		// fade back face lines
		from.color *= backFaceAttenuation;
		delta.color *= backFaceAttenuation;

		// push it back so back buffer overdraw of full strength lines can occur
		from.positionSS += backFaceOffset;
	}

	if (fabsf(GETX(delta.positionSS)) < 0.5f && GETY(delta.positionSS) < 0.5f) // it is a dot
	{
		// to do: confirm nothing? delta should be very small
	}
	else if (fabsf(GETX(delta.positionSS)) < GETY(delta.positionSS)) // diff x < diff y (note dy always > 0)
	{
		delta /= GETY(delta.positionSS);
	}
	else  // diff x >= diff y
	{
		delta /= fabsf(GETX(delta.positionSS));
	}

	// move to the first point on the screen
	while (fromY < 0 || fromX < 0 || fromX >= (int)width)
	{
		from += delta;
		fromX = ROUND_TO_INT_X(from.positionSS);
		fromY = ROUND_TO_INT_Y(from.positionSS);

		if (fromX < 0 && toX < 0) return;
		if (fromX >= (int)width && toX >= (int)width) return;
		if (fromY < 0 && toY < 0) return;
		if (fromY >= (int)height && toY >= (int)height) return;
	}

	if (GETX(delta.positionSS) > 0.0f) // left to right
	{
		while ( fromY <= toY &&			// haven't reach target y
				fromX <= toX &&			// haven't reach target x // to do: only for horizontal lines... make then a special case?
				fromY < (int)height &&	// not off the top
				fromX < (int)width)		// not off the right
		{
			if (CheckAndSetDepthBuffer(fromX, fromY, GETZ(from.positionSS)))
			{
				BEPipelinePSData psData = from;
				psData.pModel = pModel;
				psData.pEntity = pEntity;
				pixelShaderBuffer.SetValue(fromX, fromY, psData);
			}

			from += delta;

			fromX = ROUND_TO_INT_X(from.positionSS);
			fromY = ROUND_TO_INT_Y(from.positionSS);
		}
	}
	else // right to left
	{
		while ( fromY <= toY &&			// haven't reach target y
				fromX >= toX &&			// haven't reach target x // to do: only for horizontal lines... make then a special case?
				fromY < (int)height &&	// not off the top
				fromX >= 0)				// not off the left
		{
			if (CheckAndSetDepthBuffer(fromX, fromY, GETZ(from.positionSS)))
			{
				BEPipelinePSData psData = from;
				psData.pModel = pModel;
				psData.pEntity = pEntity;
				pixelShaderBuffer.SetValue(fromX, fromY, psData);
			}

			from += delta;
			
			fromX = ROUND_TO_INT_X(from.positionSS);
			fromY = ROUND_TO_INT_Y(from.positionSS);
		}
	}
}

inline void BERenderProgrammablePipeline::DrawHorizontalLineLR(BEPipelineVSData* pFromLeft, BEPipelineVSData* pToRight, BEModel* pModel, BEEntity* pEntity)
{
	// to do: may need to check it is left to right? assert

	int currentY = ROUND_TO_INT_Y(pFromLeft->positionSS); // the line

	float fromXf = GETX(pFromLeft->positionSS);
	int currentX = ROUND_TO_INT(fromXf);

	float toXf = GETX(pToRight->positionSS);
	int toX = ROUND_TO_INT(toXf);		  // end pixel to stop before

	if (currentX < 0) currentX = 0; // if left of the screen, start at the left
	if (toX > (int)width) toX = (int)width; // if the end is past the screen, stop at the end, noting we stop before this value

	BEPipelineVSData lineDelta = (*pToRight - *pFromLeft) / (toXf - fromXf); // the change per pixel

	// optimisation - get directly to the data. Easy on a horizontal line
	auto pPixel = pixelShaderBuffer.GetData(currentX, currentY);
	auto pDepth = depthBuffer.GetData(currentX, currentY);

	while (currentX < toX)
	{
		float xf = (float)currentX - 0.5f; // get the centre of the location of the pixel

		BEPipelineVSData line = *pFromLeft + lineDelta * (xf - fromXf);

		// optimised versopm of
		// if (CheckAndSetDepthBuffer(currentX, currentY, GETZ(line.positionSS)))
		//   pixelShaderBuffer.SetValue(currentX, currentY, psData);
		float z = GETZ(line.positionSS);
		if (z < *pDepth)
		{
			*pDepth = z;
			*pPixel = line;
			pPixel->pModel = pModel;
			pPixel->pEntity = pEntity;
		}

		currentX++;
		pDepth++;
		pPixel++;
	}
}

void BERenderProgrammablePipeline::DrawTriangleFlatTopLR(BEPipelineVSData* pBottom, BEPipelineVSData* pTopLeft, BEPipelineVSData* pTopRight, BEModel* pModel, BEEntity* pEntity)
{
	// draw flat tops down to bottom

	float toYf = GETY(pBottom->positionSS);
	float startLeftYf = GETY(pTopLeft->positionSS);
	float startRightYf = GETY(pTopRight->positionSS);

	int currentY = ROUND_TO_INT(startLeftYf) - 1; // value to start at. Going opposite direciton so start -1
	int toY = ROUND_TO_INT(toYf);			 // value to stop at. Going opposite direciton stop at, not before

	if (toY >= (int)height) return; // triangle ending above the screen
	if (currentY < 0.0f) return;	// triangle starting below the screen

	if (toY < 0) toY = 0; // if end is below the screen, change the toY to be the end
	if (currentY >= (int)height) currentY = (int)height - 1;

	BEPipelineVSData lineStartDelta = (*pBottom - *pTopLeft) / (startLeftYf - toYf);
	BEPipelineVSData lineEndDelta = (*pBottom - *pTopRight) / (startRightYf - toYf);

	// draw each horizontal line
	while (currentY >= toY)
	{
		float yf = (float)currentY + 0.5f; // get the centre location of the pixel

		BEPipelineVSData lineStart = *pTopLeft + lineStartDelta * (startLeftYf - yf);
		BEPipelineVSData lineEnd = *pTopRight + lineEndDelta * (startRightYf - yf);
		DrawHorizontalLineLR(&lineStart, &lineEnd, pModel, pEntity);

		currentY--;
	}
}

void BERenderProgrammablePipeline::DrawTriangleFlatBottomLR(BEPipelineVSData* pBottomLeft, BEPipelineVSData* pBottomRight, BEPipelineVSData* pTop, BEModel* pModel, BEEntity* pEntity)
{
	// draw flat bottoms up to top

	float toYf = GETY(pTop->positionSS);
	float startLeftYf = GETY(pBottomLeft->positionSS);
	float startRightYf = GETY(pBottomRight->positionSS);

	int currentY = ROUND_TO_INT(startLeftYf); // value to start at.
	int toY = ROUND_TO_INT(toYf);			  // value to stop before

	if (currentY >= (int)height) return; // triangle starts above the screen
	if (toY < 0) return;				 // triangle ends below the screen

	if (toY > (int)height) toY = (int)height; // end is above the screen, so stop at the top row, which is before height
	if (currentY < 0) currentY = 0;			  // starting below the screen, so start at 0

	BEPipelineVSData lineStartDelta = (*pTop - *pBottomLeft) / (toYf - startLeftYf);
	BEPipelineVSData lineEndDelta = (*pTop - *pBottomRight) / (toYf - startRightYf);

	// draw each horizontal line
	while (currentY < toY)
	{
		float yf = (float)currentY + 0.5f; // get the centre location of the pixel

		BEPipelineVSData lineStart = *pBottomLeft + lineStartDelta * (yf - startLeftYf);
		BEPipelineVSData lineEnd = *pBottomRight + lineEndDelta * (yf - startRightYf);
		DrawHorizontalLineLR(&lineStart, &lineEnd, pModel, pEntity);

		currentY++;
	}
}

inline DirectX::XMVECTOR BERenderProgrammablePipeline::ScreenSpaceToPixelCoord(DirectX::XMVECTOR v)
{
	return (v + XMVECTOR({1.0f, 1.0f, 0.0f, 0.0f})) * XMVECTOR({widthHalf, heightHalf, 1.0f, 1.0f});
}

inline bool BERenderProgrammablePipeline::IsOnCanvas(DirectX::XMVECTOR& v)
{
	// to do: consider rounding? ie width -0.0f
	return GETX(v) >= 0.0f && GETX(v) < width && GETY(v) >= 0.0f && GETY(v) < height;
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
