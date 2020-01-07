#include "BERenderProgrammablePipeline.h"

using namespace DirectX;

#define GETX(v) XMVectorGetX(v)
#define GETY(v) XMVectorGetY(v)
#define GETZ(v) XMVectorGetZ(v)
#define ROUND_TO_INT(f) (int)roundf(f)
#define ROUND_TO_INT_X(v) (int)roundf(GETX(v))
#define ROUND_TO_INT_Y(v) (int)roundf(GETY(v))
#define ROUND_TO_INT_Z(v) (int)roundf(GETZ(v))

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

inline BEPipelineVSData& BEPipelineVSData::operator+=(const BEPipelineVSData& rhs)
{
	positionWS += rhs.positionWS;
	normalWS += rhs.normalWS;
	positionSS += rhs.positionSS;
	color += rhs.color;
	texcoord.x += rhs.texcoord.x;
	texcoord.y += rhs.texcoord.y;
	return *this;
}

inline BEPipelineVSData& BEPipelineVSData::operator/=(const float rhs)
{
	positionWS /= rhs;
	normalWS /= rhs;
	positionSS /= rhs;
	color /= rhs;
	texcoord.x /= rhs;
	texcoord.y /= rhs;
	return *this;
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
	memset(vsBuffer, 0, sizeof(BEPipelineVSData) * vsBufferSize);

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
	depthBuffer.Clear(FLT_MAX);
	//message.str(std::wstring());
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
	pOutput->positionSS = ScreenSpaceToPixelCoord(pCamera->WorldToScreen(pOutput->positionWS));
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
			BEPipelineVSData* pv0 = pVSData++;
			BEPipelineVSData* pv1 = pVSData++;
			BEPipelineVSData* pv2 = pVSData++;

			//RasterizerPoints(pv0, pv1, pv2, pModel, pEntity);
			RasterizerWireframe(pv0, pv1, pv2, pModel, pEntity);
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
			RasterizerWireframe(pv0, pv1, pv2, pModel, pEntity);
			indx += 3;
		}
	}
}

bool BERenderProgrammablePipeline::Cull(BEPipelineVSData* pv0, BEPipelineVSData* pv1, BEPipelineVSData* pv2)
{
	if (IsOnCanvas(pv0->positionSS) ||
		IsOnCanvas(pv1->positionSS) ||
		IsOnCanvas(pv2->positionSS)
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

void BERenderProgrammablePipeline::PixelShading()
{
	for (unsigned int y = 0; y < pCanvas->height; y++)
	{
		for (unsigned int x = 0; x < pCanvas->width; x++)
		{
			XMVECTOR color; //to do: later write direct to buffer?
			PixelShader(pixelShaderBuffer.GetData(x, y), &color);
			pCanvas->bufferSurface->SetValue(x, y, color);
		}
	}
}

void BERenderProgrammablePipeline::PixelShader(BEPipelinePSData* pPSData, DirectX::XMVECTOR* pOutput)
{
	*pOutput = pPSData->color;
}

void BERenderProgrammablePipeline::DrawPoint(BEPipelineVSData* pVS, BEModel* pModel, BEEntity* pEntity, bool backFace)
{
	int x = ROUND_TO_INT_X(pVS->positionSS);
	if (x < 0 || x >= (int)width) return;

	int y = ROUND_TO_INT_Y(pVS->positionSS);
	if (y < 0 || y >= (int)height) return;

	float z;
	if (backFace) z = GETZ(pVS->positionSS + backFaceOffset);
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
	//if (depthBuffer.GetValue(x, y) > depth) {
	//	depthBuffer.SetValue(x, y, depth);
	//	return true;
	//}
	//else return false;

	// to do: faster to do this?
	auto pData = depthBuffer.GetData(x, y);
	if (depth < *pData)
	{
		*pData = depth;
		return true;
	}
	else return false;
}

// sort
//if (GETY(pv0->positionSS) > GETY(pv1->positionSS)) std::swap(pv0, pv1);
//if (GETY(pv1->positionSS) > GETY(pv2->positionSS)) {
//	std::swap(pv1, pv2);
//	if (GETY(pv0->positionSS) > GETY(pv1->positionSS)) std::swap(pv0, pv1);
//}
