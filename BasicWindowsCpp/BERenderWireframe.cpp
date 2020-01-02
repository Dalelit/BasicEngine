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

	for (BEModel* model : pScene->models)
	{
		BEMesh* m = model->pMesh; // get it's mesh

		for (BEEntity* entity : model->entities) // loop on each instance of it
		{
			if (m) // if it has a mesh
			{
				// create screen space version of all verticies
				{
					BEVertex* src = m->verticies;
					BEVertex* tgt = screenSpaceVerticies;
					for (unsigned int i = 0; i < m->vertCount; i++)
					{
						// to do: merge the matricies into a single transform
						XMVECTOR tgtPos = entity->ModelToWorldPosition(src->position);	// model to world space
						tgt->position = pCamera->WorldToScreen(tgtPos);					// world to screen space
						tgt->color = src->color;
						tgt++;
						src++;
					}
				}

				for (unsigned int i = 0; i < m->triCount; i++) // look at each triangle
				{
					// get the normal for 1 vertex to see if it's visible... will get the rest later if needed
					XMVECTOR n0 = m->verticies[m->triangles[i].indx[0]].normal;
					n0 = XMVector3Normalize(entity->ModelToWorldDirection(n0)); // model to world direction

					// to do: work out if there is a more efficient way?
					bool isVisible = pCamera->IsVisible(entity->ModelToWorldPosition(m->verticies[m->triangles[i].indx[0]].position), n0);

					if (!backfaceCull || isVisible)
					{
						XMVECTOR c = lineColor;

						// get all the vertecies
						XMVECTOR v0 = screenSpaceVerticies[m->triangles[i].indx[0]].position;
						XMVECTOR v1 = screenSpaceVerticies[m->triangles[i].indx[1]].position;
						XMVECTOR v2 = screenSpaceVerticies[m->triangles[i].indx[2]].position;

						// check it's in the screen bounds
						if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1) || pCamera->OveralpsScreen(v2))
						{
							// draw back faces with different color now if culling them
							// and draw full strength lines later
							if (!backfaceCull && isVisible)
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

							if (drawNormals)
							{
								// get the rest of the normals
								XMVECTOR n1 = m->verticies[m->triangles[i].indx[1]].normal;
								XMVECTOR n2 = m->verticies[m->triangles[i].indx[2]].normal;
								n0 = n0 * normalLength;
								n1 = XMVector3Normalize(entity->ModelToWorldDirection(n1)) * normalLength;
								n2 = XMVector3Normalize(entity->ModelToWorldDirection(n2)) * normalLength;

								pCanvas->DrawLineSafe(v0, v0 + pCamera->WorldToScreen(n0), normalColor);
								pCanvas->DrawLineSafe(v1, v1 + pCamera->WorldToScreen(n1), normalColor);
								pCanvas->DrawLineSafe(v2, v2 + pCamera->WorldToScreen(n2), normalColor);
							}
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
			}
		}
	}
}
