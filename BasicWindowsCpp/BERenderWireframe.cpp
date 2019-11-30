#include "BERenderPipeline.h"

//
// To Do
// - could add depth buffer so the lines overlap correctly, not just based on enitity/model order?

BERenderPipelineWireframe::BERenderPipelineWireframe(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pScene = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;

	// pre allocate memory
	screenSpaceVerticies = new BEVertex[BERENDERPIPELINE_MAX_VERTICES];
}

BERenderPipelineWireframe::~BERenderPipelineWireframe()
{
	delete screenSpaceVerticies;
}

void BERenderPipelineWireframe::Draw()
{
	for (unsigned int eindx = 0; eindx < pScene->entityCount; eindx++) // for each entity
	{
		BEMesh* m = pScene->entities[eindx]->mesh; // get it's mesh

		XMVECTOR* screenSpaceNormals = NULL;

		if (m) // if it has a mesh
		{
			// create screen space version of all verticies
			{
				BEVertex* src = m->verticies;
				BEVertex* tgt = screenSpaceVerticies;
				for (unsigned int i = 0; i < m->vertCount; i++)
				{
					tgt->position = pCamera->WorldToScreen(src->position);
					tgt->color = src->color;
					tgt++;
					src++;
				}

				//screenSpaceNormals = tgt;
				//src = m->normals;

				//if (drawNormals || backfaceCull)
				//{
				//	for (unsigned int nindx = 0; nindx < m->nCount; nindx++)
				//	{
				//		*tgt = pCamera->WorldToScreen((m->verticies[nindx * 3] + *src) * 0.1f);
				//		tgt++;
				//		src++;
				//	}
				//}
			}

			for (unsigned int i = 0; i < m->triCount; i++) // look at each triangle
			{
				XMVECTOR v0 = screenSpaceVerticies[m->triangles[i].indx[0]].position;
				XMVECTOR v1 = screenSpaceVerticies[m->triangles[i].indx[1]].position;
				XMVECTOR v2 = screenSpaceVerticies[m->triangles[i].indx[2]].position;

				XMVECTOR normal = m->triangles[i].normal;
				XMVECTOR v0n = XMVector3Normalize(-v0);
				XMVECTOR c = screenSpaceVerticies[m->triangles[i].indx[2]].color;

				XMVECTOR ssNormal = XMVector3Normalize( XMVector3Cross( (v1 - v0), (v2 - v0) ) );

				// check it's in the screen bounds
				if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1) || pCamera->OveralpsScreen(v2))
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

			// To do : fix line drawing
			//unsigned int lindx = 0;
			//while (lindx < m->lBufferSize)
			//{
			//	XMVECTOR v0 = screenSpaceVerticies[m->lines[lindx++]];
			//	XMVECTOR v1 = screenSpaceVerticies[m->lines[lindx++]];

			//	XMVECTOR c = pScene->entities[eindx]->color;

			//	// check it's in the screen bounds
			//	if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1))
			//	{
			//		pCanvas->DrawLineSafe(v0, v1, c);
			//	}
			//}
		}
	}
}
