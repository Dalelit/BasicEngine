#include "BERenderPipeline.h"

using namespace DirectX;

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
	linesToDraw = new BELineInfo[BERENDERPIPELINE_MAX_TRIEDGES]; // to do: sensible number?
}

BERenderPipelineWireframe::~BERenderPipelineWireframe()
{
	delete screenSpaceVerticies;
	delete linesToDraw;
}

void BERenderPipelineWireframe::Draw()
{
	unsigned int lineCounter = 0; // store line info so back face lines are drawn first, the final lines last

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
			}

			for (unsigned int i = 0; i < m->triCount; i++) // look at each triangle
			{
				//XMVECTOR normal = m->triangles[i].normal;
				XMVECTOR normal = m->verticies[m->triangles[i].indx[0]].normal; // to do: only using the first normal for now

				bool isVisible = pCamera->IsVisible(m->verticies[m->triangles[i].indx[0]].position, normal);

				if (!backfaceCull || isVisible)
				{
					XMVECTOR v0 = screenSpaceVerticies[m->triangles[i].indx[0]].position;
					XMVECTOR v1 = screenSpaceVerticies[m->triangles[i].indx[1]].position;
					XMVECTOR v2 = screenSpaceVerticies[m->triangles[i].indx[2]].position;

					XMVECTOR v0n = XMVector3Normalize(-v0);
					XMVECTOR c = lineColor; // screenSpaceVerticies[m->triangles[i].indx[2]].color;

					XMVECTOR ssNormal = XMVector3Normalize(XMVector3Cross((v1 - v0), (v2 - v0)));

					// check it's in the screen bounds
					if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1) || pCamera->OveralpsScreen(v2))
					{
						if (!backfaceCull && isVisible) // draw later
						{
							linesToDraw[lineCounter].v0 = v0;
							linesToDraw[lineCounter].v1 = v1;
							linesToDraw[lineCounter].v2 = v2;
							linesToDraw[lineCounter].color = c;
							lineCounter++;
						}
						else // draw backface now
						{
							if (!isVisible) c = backlineColor;
							pCanvas->DrawLineSafe(v0, v1, c);
							pCanvas->DrawLineSafe(v1, v2, c);
							pCanvas->DrawLineSafe(v2, v0, c);
						}

						//if (drawNormals)
						//{
						//	pCanvas->DrawLineSafe(v0, v0 + ssNormal, normalColor);
						//	pCanvas->DrawLineSafe(v1, v1 + ssNormal, normalColor);
						//	pCanvas->DrawLineSafe(v2, v2 + ssNormal, normalColor);
						//}
					}
				}
			}

			// draw main lines
			for (unsigned int i = 0; i < lineCounter; i++)
			{
				pCanvas->DrawLineSafe(linesToDraw[i].v0, linesToDraw[i].v1, linesToDraw[i].color);
				pCanvas->DrawLineSafe(linesToDraw[i].v1, linesToDraw[i].v2, linesToDraw[i].color);
				pCanvas->DrawLineSafe(linesToDraw[i].v2, linesToDraw[i].v0, linesToDraw[i].color);
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
