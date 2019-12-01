#include "BERenderPipeline.h"
#include <DirectXCollision.h>

BERenderPipelineRaytrace::BERenderPipelineRaytrace(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pScene = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;
}

BERenderPipelineRaytrace::~BERenderPipelineRaytrace()
{
}

void BERenderPipelineRaytrace::InnerLoop(float px, float py, unsigned int x, unsigned int y, unsigned int line)
{
	BECamera::Ray r = pCamera->RelativeScreenPositionToRay(px, py);
	float hitDistance = pCamera->maxDistance; // to do: what distance is the max starting?
	float distance;

	for (unsigned int eindx = 0; eindx < pScene->entityCount; eindx++) // for each entity
	{
		BEMesh* m = pScene->entities[eindx]->mesh; // get it's mesh

		if (m) // if it has a mesh
		{
			for (unsigned int i = 0; i < m->triCount; i++) // look at each triangle
			{
				XMVECTOR normal = m->triangles[i].normal;
				XMVECTOR v0 = m->verticies[m->triangles[i].indx[0]].position;

				if (!backfaceCull || pCamera->IsVisible(v0, normal))
				{
					XMVECTOR v1 = m->verticies[m->triangles[i].indx[1]].position;
					XMVECTOR v2 = m->verticies[m->triangles[i].indx[2]].position;

					if (TriangleTests::Intersects(r.position, r.direction, v0, v1, v2, distance))
					{
						if (distance < hitDistance)
						{
							// To Do : not actually thought about this yet.

							// To Do : assuming all color is on the 1st vertex
							XMVECTOR color = m->verticies[m->triangles[i].indx[0]].color;
							XMVECTOR ambient = color;

							XMVECTOR lights = { 0,0,0,1 };

							//XMVECTOR normal = XMVector3Normalize( XMVector3Cross ( (v1 - v0), (v2 - v0) ) );

							for (unsigned int lindx = 0; lindx < pScene->lightCount; lindx++)
							{
								lights += pScene->lights[lindx]->CalculateColor(normal);
							}

							lights = lights / (float)pScene->lightCount;

							XMVECTOR c = XMVectorSaturate(0.5f * ambient + 0.5f * lights);

							pCanvas->buffer[line + x] = c;
							hitDistance = distance;
						}
					}
				}
			}
		}
	}
}

void BERenderPipelineRaytrace::Draw()
{
	//DrawByLine();
	DrawBySampling();
}

void BERenderPipelineRaytrace::DrawByLine()
{
	raysToProcess = pCanvas->size;
	raysProcessed = 0;

	float dx = 2.0f / pCanvas->width;
	float dy = 2.0f / pCanvas->height;
	float px = -1;
	float py = -1;

	int line = 0;

	for (unsigned int y = 0; y < pCanvas->height; y++)
	{
		px = -1;

		for (unsigned int x = 0; x < pCanvas->width; x++)
		{
			InnerLoop(px, py, x, y, line);

			px += dx;

			showBuffer = true; // every full line show the buffer

			raysProcessed++;

			if (exitLoop || restartLoop) return;
		}

		py += dy;
		line += pCanvas->width;
	}
}

// to do: tidy up types... check for efficiency
void BERenderPipelineRaytrace::DrawBySampling()
{
	raysToProcess = pCanvas->size;
	raysProcessed = 0;

	int size = (int)pCanvas->size;
	int width = (int)pCanvas->width;

	float px, py;
	unsigned int x, y;

	int indx = 0;
	int offset = 13;
	int startingOffset = offset - 1;

	float invWidthx2 = 2.0f / (float)pCanvas->width;
	float invHeightx2 = 2.0f / (float)pCanvas->height;

	int counter = 0;

	while (startingOffset >= 0)
	{
		int indx = startingOffset;

		while (indx < size)
		{
			x = indx % width;
			y = indx / width;
			px = (float)x * invWidthx2 - 1.0f;
			py = (float)y * invHeightx2 - 1.0f;
			InnerLoop(px, py, x, y);
			counter++;

			if (counter % 10 == 0)
			{
				showBuffer = true;
				if (exitLoop || restartLoop) return;
			}

			indx += offset;
			raysProcessed++;
		}

		startingOffset--;
	}
}
