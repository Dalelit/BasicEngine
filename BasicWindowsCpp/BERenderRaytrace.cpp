#include "BERenderPipeline.h"
#include "BERaytrace.h"
#include <DirectXCollision.h>

using namespace DirectX;

BERenderPipelineRaytrace::BERenderPipelineRaytrace(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pScene = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;
	stride = pCanvas->width;
	invWidthx2 = 2.0f / (float)pCanvas->width;
	invHeightx2 = 2.0f / (float)pCanvas->height;
	raysToProcess = pCanvas->width * pCanvas->height;
	raysProcessed = 0;
}

BERenderPipelineRaytrace::~BERenderPipelineRaytrace()
{
}

// to do: sampling?
void BERenderPipelineRaytrace::InnerLoop(unsigned int x, unsigned int y)
{
	float px = (float)x * invWidthx2 - 1.0f;
	float py = (float)y * invHeightx2 - 1.0f;

	BECamera::Ray rayWorldSpace = pCamera->RelativeScreenPositionToRay(px, py); // create the ray

	float hitDistance = pCamera->maxDistance; // to do: what distance is good for the starting max?

	BERaytraceHit hitInfo;

	// if we hit, work out the color.
	if (BERaytrace::RayHit(pScene, pCamera, rayWorldSpace.origin, rayWorldSpace.direction, hitDistance, hitInfo))
	{
		XMVECTOR color;

		if (hitInfo.pModel->pMesh->IsTextured())
		{
			// texture sampler
			XMFLOAT2 texcoord = BEXMFloat2BaryCentric(
				hitInfo.pV0->texcoord, hitInfo.pV1->texcoord, hitInfo.pV2->texcoord,
				hitInfo.u, hitInfo.v);
			color = hitInfo.pModel->pMesh->pTextureSampler->SampleClosest(texcoord);
		}
		else
		{
			// color
			color = XMVectorBaryCentric(
				hitInfo.pV0->color, hitInfo.pV1->color, hitInfo.pV2->color,
				hitInfo.u, hitInfo.v);
		}

		XMVECTOR positionWS = XMVectorBaryCentric(
				hitInfo.pV0->position, hitInfo.pV1->position, hitInfo.pV2->position,
				hitInfo.u, hitInfo.v);
		hitInfo.pEntity->ModelToWorldPosition(positionWS);

		XMVECTOR normalWS = XMVectorBaryCentric(
			hitInfo.pV0->normal, hitInfo.pV1->normal, hitInfo.pV2->normal,
			hitInfo.u, hitInfo.v);
		hitInfo.pEntity->ModelToWorldDirection(normalWS);

		XMVECTOR lights = pScene->ambientLight;
		lights += pScene->directionalLight.CalculateColorInWorldSpace(normalWS);

		for (BEPointLight* pLight : pScene->lights)
		{
			if (!BERaytrace::TargetOccluded(pScene, positionWS, pLight->position))
			{
				lights += pLight->CalculateColorInWorldSpace(positionWS, normalWS);
			}
		}

		// to do: reflections?

		pCanvas->buffer[y * stride + x] = XMVectorSaturate(lights * color);
	}
	//else pCanvas->buffer[y * stride + x] = { 1,0,0,1 }; // set color on no hit - for debug
}

void BERenderPipelineRaytrace::Draw()
{
	//DrawByLine();
	DrawBySampling(0, pCanvas->width, 0, pCanvas->height);
}

void BERenderPipelineRaytrace::Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height)
{
	DrawBySampling(xFrom, width, yFrom, height);
}

void BERenderPipelineRaytrace::DrawByLine()
{
	for (unsigned int y = 0; y < pCanvas->height; y++)
	{
		for (unsigned int x = 0; x < pCanvas->width; x++)
		{
			InnerLoop(x, y);

			showBuffer = true; // every full line show the buffer

			raysProcessed++;

			if (exitLoop || restartLoop) return;
		}
	}
}

// to do: tidy up types... check for efficiency
void BERenderPipelineRaytrace::DrawBySampling(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height)
{
	int size = width * height;
	unsigned int x, y;

	int indx = 0;
	int offset = 13;
	int startingOffset = offset - 1;

	int counter = 0;

	while (startingOffset >= 0)
	{
		int indx = startingOffset;

		while (indx < size)
		{
			x = indx % width;
			y = indx / width;
			InnerLoop(xFrom + x, yFrom + y);
			counter++;

			if (counter % 10 == 0)
			{
				showBuffer = true;
				if (exitLoop || restartLoop) return;
			}

			indx += offset;
			raysProcessed++; // to do: not thread safe?
		}

		startingOffset--;
	}
}
