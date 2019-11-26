#include "BERenderPipeline.h"

//
// To Do
// - could add depth buffer so the lines overlap correctly, not just based on enitity/model order?

BERenderPipelineWireframe::BERenderPipelineWireframe(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pWorld = _pWorld;
	pCamera = _pCamera;
	pCanvas = _pCanvas;

	// pre allocate memory
	screenSpaceVerticies = new XMVECTOR[BERENDERPIPELINE_MAX_VERTICES];
}

BERenderPipelineWireframe::~BERenderPipelineWireframe()
{
	delete screenSpaceVerticies;
}

void BERenderPipelineWireframe::Draw()
{
	for (unsigned int eindx = 0; eindx < pWorld->entityCount; eindx++) // for each entity
	{
		BEMesh* m = pWorld->entities[eindx]->mesh; // get it's mesh

		XMVECTOR* screenSpaceNormals = NULL;

		if (m) // if it has a mesh
		{
			// create screen space version of all verticies
			{
				XMVECTOR* src = m->verticies;
				XMVECTOR* tgt = screenSpaceVerticies;
				for (unsigned int vindx = 0; vindx < m->vCount; vindx++)
				{
					*tgt = pCamera->WorldToScreen(*src);
					tgt++;
					src++;
				}

				screenSpaceNormals = tgt;
				src = m->normals;

				if (drawNormals || backfaceCull)
				{
					for (unsigned int nindx = 0; nindx < m->nCount; nindx++)
					{
						*tgt = pCamera->WorldToScreen((m->verticies[nindx * 3] + *src) * 0.1f);
						tgt++;
						src++;
					}
				}
			}

			unsigned int tindx = 0;
			unsigned int trinum = 0;

			while (tindx < m->tBufferSize) // look at each triangle
			{
				XMVECTOR v0 = screenSpaceVerticies[m->triangles[tindx++]];
				XMVECTOR v1 = screenSpaceVerticies[m->triangles[tindx++]];
				XMVECTOR v2 = screenSpaceVerticies[m->triangles[tindx++]];

				XMVECTOR normal = m->normals[trinum];
				//Vector3 toCamera = -v0;
				//float dot = pCamera->DirectionDot(normal);
				//float dot = normal.Dot(toCamera);
				//float dot = toCamera.Dot(normal);
				XMVECTOR v0n = XMVector3Normalize(-v0);
				//v0n.Normalize();
				//if (normal.Dot(v0n) < 0.0f) // || normal.Dot(-v1) < 0.0f || normal.Dot(-v2) < 0.0f)
				{

					XMVECTOR c = pWorld->entities[eindx]->color;

					XMVECTOR ssNormal = XMVector3Normalize( XMVector3Cross( (v1 - v0), (v2 - v0) ) );
					ssNormal *= 5.0f;

					// check it's in the screen bounds
					if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1) || pCamera->OveralpsScreen(v2))
					{
						//if (Vector3::Forward.Dot(ssNormal) > 0.0f)
						{
							pCanvas->DrawLineSafe(v0, v1, c);
							pCanvas->DrawLineSafe(v1, v2, c);
							pCanvas->DrawLineSafe(v2, v0, c);

							if (drawNormals)
							{
								pCanvas->DrawLineSafe(v0, v0 + ssNormal, normalColor);
								pCanvas->DrawLineSafe(v1, v1 + ssNormal, normalColor);
								pCanvas->DrawLineSafe(v2, v2 + ssNormal, normalColor);
							}
						}
					}
				}

				trinum++;
			}

			unsigned int lindx = 0;
			while (lindx < m->lBufferSize)
			{
				XMVECTOR v0 = screenSpaceVerticies[m->lines[lindx++]];
				XMVECTOR v1 = screenSpaceVerticies[m->lines[lindx++]];

				XMVECTOR c = pWorld->entities[eindx]->color;

				// check it's in the screen bounds
				if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1))
				{
					pCanvas->DrawLineSafe(v0, v1, c);
				}
			}
		}
	}
}
