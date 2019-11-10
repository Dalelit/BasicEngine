#include "BERenderPipeline.h"

BERenderPipelineRaytrace::BERenderPipelineRaytrace(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pWorld = _pWorld;
	pCamera = _pCamera;
	pCanvas = _pCanvas;
}

BERenderPipelineRaytrace::~BERenderPipelineRaytrace()
{
}

void BERenderPipelineRaytrace::Draw()
{
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
			Ray r = pCamera->RelativeScreenPositionToRay(px, py);
			float hitDistance = 10000.0f; // to do: what distance is the max starting?
			float distance;

			for (unsigned int eindx = 0; eindx < pWorld->entityCount; eindx++) // for each entity
			{
				BEMesh* m = pWorld->entities[eindx]->mesh; // get it's mesh

				if (m) // if it has a mesh
				{
					unsigned int tindx = 0;

					while (tindx < m->tCount) // look at each triangle
					{
						Vector3 v0 = m->verticies[m->triangles[tindx++]];
						Vector3 v1 = m->verticies[m->triangles[tindx++]];
						Vector3 v2 = m->verticies[m->triangles[tindx++]];

						if (r.Intersects(v0, v1, v2, distance))
						{
							if (distance < hitDistance)
							{
								pCanvas->buffer[line + x] = pWorld->entities[eindx]->color;
								hitDistance = distance;
							}
						}
					}
				}
			}

			px += dx;
		}

		py += dy;
		line += pCanvas->width;
	}
}
