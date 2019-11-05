#include "BEHelpers.h"
#include "BERenderPipeline.h"

BERenderPipeline::BERenderPipeline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pWorld = _pWorld;
	pCamera = _pCamera;
	pCanvas = _pCanvas;
}

void BERenderPipeline::UpdateScreenSpace()
{
	for (unsigned int m = 0; m < pWorld->modelCount; m++)
	{
		BEModel* pModel = pWorld->models[m];
		if (pModel != 0)
		{
			for (unsigned int v = 0; v < pModel->vCount; v++)
			{
				pModel->screenPoints[v] = pCamera->WorldToScreen(pModel->verticies[v]);
			}
		}
	}
}

void BERenderPipeline::Draw()
{
	// loop through each entity/mesh/triangle
	//   convert to screen space
	//   if inside screen space, then create edges in pixel space
	// draw edges

	BETriEdge* sortedList = NULL;

	// create sorted list of triangle-edges
	{
		BEEdge* edge = edges;
		BETriEdge* triedage = triedges;

		for (unsigned int eindx = 0; eindx < pWorld->entityCount; eindx++) // for each entity
		{
			BEMesh* m = pWorld->entities[eindx]->mesh; // get it's mesh

			if (m) // if it has a mesh
			{
				// create screen space version of all verticies
				{
					Vector3* src = m->verticies;
					Vector3* tgt = screenSpaceVerticies;
					for (unsigned int vindx = 0; vindx < m->vCount; vindx++)
					{
						*tgt = pCamera->WorldToScreen(*src);
						tgt++;
						src++;
					}
				}

				unsigned int tindx = 0;

				while (tindx < m->tCount) // look at each triangle
				{
					Vector3 v0 = screenSpaceVerticies[m->triangles[tindx++]];
					Vector3 v1 = screenSpaceVerticies[m->triangles[tindx++]];
					Vector3 v2 = screenSpaceVerticies[m->triangles[tindx++]];
					Color c0 = { 1,0,0 };
					Color c1 = { 0,1,0 };
					Color c2 = { 0,0,1 };

					// check it's in the screen bounds
					if (pCamera->IsOnScreen(v0) && pCamera->IsOnScreen(v1) && pCamera->IsOnScreen(v2))
					{
						// convert to pixel coords
						v0 = pCanvas->ScreenToPixel(v0);
						v1 = pCanvas->ScreenToPixel(v1);
						v2 = pCanvas->ScreenToPixel(v2);

						// order the verticies lowest to highest // To do - is there something more efficient?
						Vector3 tmp;
						if (v0.y > v1.y) SWAPVECTOR3(v0, v1)
						if (v1.y > v2.y)
						{
							SWAPVECTOR3(v1, v2)
							if (v0.y > v1.y) SWAPVECTOR3(v0, v1)
						}

						// create the edges and triedge
						InitEdge(edge, &v0, &v1, c0, c1);
						triedage->e0 = edge;
						triedage->yStart = (unsigned int)v0.y;
						edge++;
						InitEdge(edge, &v0, &v2, c0, c2);
						triedage->e1 = edge;
						edge++;
						InitEdge(edge, &v1, &v2, c1, c2);
						edge++;

						// special case where first edge e0 is flat... replace it with e2
						// scan lines will then both stop for e0, e1 at the same time
						if (v0.y == v1.y) triedage->e0 += 2;

						// insert the triedge into a sorted list
						if (!sortedList) // first one in the list to add
						{
							sortedList = triedage;
							triedage->next = NULL;
						}
						else
						{
							BETriEdge* current = sortedList;
							BETriEdge* previous = NULL;

							while (current && current->yStart < triedage->yStart) // find the point to insert
							{
								previous = current;
								current = current->next;
							}
						
							if (!current) // reached the end of the list
							{
								previous->next = triedage;
								triedage->next = NULL;
							}
							else
							{
								if (previous == NULL) // inserting at the beginning of the list
								{
									sortedList = triedage;
								}
								else
								{
									previous->next = triedage;
								}
								triedage->next = current;
							}
						}

						triedage++;
					}
				}
			}
		}
	} // sorted list of triangle edges created

	if (sortedList == 0) return; // nothing to draw

	// now we have the edges, draw them
	{
		// put the first edge in the active list
		BETriEdge* activeList = sortedList;
		sortedList = sortedList->next;
		activeList->next = NULL;
		unsigned int y = activeList->yStart;
		unsigned int line = y * pCanvas->width;

		while ((sortedList || activeList) && y < pCanvas->height)
		{
			// add to active list
			while (sortedList && sortedList->yStart <= y)
			{
				BETriEdge* move = sortedList;
				sortedList = sortedList->next;
				move->next = activeList;
				activeList = move;
			}

			// draw
			BETriEdge* current = activeList;

			if (y >= 0)
			{
				while (current)
				{
					unsigned int x = (unsigned int)current->e0->x;
					unsigned int xt = (unsigned int)current->e1->x;

					float z = current->e0->z;
					float dz = (current->e1->z - current->e0->z) / (current->e1->z - current->e0->z);

					Color c = current->e0->c;
					Color dc = (current->e1->c - current->e0->c) / (current->e1->x - current->e0->x);

					if (xt < x)
					{
						SWAPINT(x, xt);

						z = current->e1->z;
						dz = -dz;

						c = current->e1->c;
						dc = -dc;
					}

					if (x < 0) x = 0; // move onto the screen if off it

					while (x <= xt && x < pCanvas->width)
					{
						pCanvas->buffer[line + x] = c;
						x++;
						c += dc;
					}

					current = current->next;
				}
			}

			// remove edges from active if finished or update them
			current = activeList;
			BETriEdge* previous = NULL;
			while (current)
			{
				if (current->e0->yEnd == y) // e0 finished so...
				{
					if (current->e1->yEnd == y) // e0 and e1 finished, so the edge is finished
					{
						if (previous == NULL)
						{
							activeList = current->next; // removing the first one in the list
						}
						else
						{
							previous->next = current->next; // or just removing one in the middle
						}
					}
					else // e0 finished, e1 not
					{
						current->e0 += 2; // e0 becomes e2
						UpdateEdge(current->e1);
						previous = current;
					}
				}
				else
				{

					if (current->e1->yEnd == y) // e1 finished, not e0
					{
						UpdateEdge(current->e0);
						current->e1 += 1; // e1 becomes e2
					}
					else // e0 and e1 not finished
					{
						UpdateEdge(current->e0);
						UpdateEdge(current->e1);
					}
					previous = current;
				}
				current = current->next;
			}

			y++;
			line += pCanvas->width;
		}
	} // done drawing
}

void BERenderPipeline::DrawV1()
{
	BEModel* pModel;

	for (unsigned int m = 0; m < pWorld->modelCount; m++)
	{
		pModel = pWorld->models[m];
		if (pModel != 0)
		{
			for (unsigned int l = 0; l < pModel->lCount;)
			{
				unsigned int indx1 = l++;
				unsigned int indx2 = l++;

				Vector3* pv1 = pModel->screenPoints + pModel->lines[indx1];
				Vector3* pv2 = pModel->screenPoints + pModel->lines[indx2];
				if (pv1->z > 0.0f && pv2->z > 0.0f) // cull line is either point is off the viewer
				{
					if (pModel->colors == NULL)
					{
						pCanvas->DrawLineSafe(*pv1, *pv2, pModel->color);
					}
					else
					{
						pCanvas->DrawLineSafe(*pv1, *pv2, *(pModel->colors + pModel->lines[indx1]), *(pModel->colors + pModel->lines[indx2]));
					}
				}
			}
		}
	}
}

void BERenderPipeline::DrawModel(BEModel* pModel)
{
	unsigned int i = 0;
	while (i < pModel->lCount)
	{
		Vector3* pv1 = pModel->screenPoints + pModel->lines[i++];
		Vector3* pv2 = pModel->screenPoints + pModel->lines[i++];
		if (pv1->z > 0.0f && pv2->z > 0.0f)
		{
			pCanvas->DrawLineSafe(*pv1, *pv2, pModel->color);
		}
	}
}

//void BERenderPipeline::DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color)
//{
//	Vector3* v[3];
//	v[0] = &v1;
//	v[1] = &v2;
//	v[2] = &v3;
//
//	// sort - To Do - rethink this
//	Vector3* tmp;
//	if (v[0]->y > v[1]->y) { tmp = v[0]; v[0] = v[1]; v[1] = tmp; }
//	if (v[1]->y > v[2]->y) { tmp = v[1]; v[1] = v[2]; v[2] = tmp; }
//	if (v[0]->y > v[1]->y) { tmp = v[0]; v[0] = v[1]; v[1] = tmp; }
//
//	BEEdge edges[3];
//	InitEdge(edges,   v[0], v[1]);
//	InitEdge(edges+1, v[0], v[2]);
//	InitEdge(edges+2, v[1], v[2]);
//
//	unsigned int y = (int)v[0]->y;
//
//	if (y == edges[0].yEnd) // special case of the first edge being flat
//	{
//		DrawScanLine(y, (int)edges[0].x, (int)edges[0].dx, color);
//		y++;
//	}
//	else while (y <= edges[0].yEnd)
//	{
//		DrawScanLine(y, (int)edges[0].x, (int)edges[1].x, color);
//		edges[0].x += edges[0].dx;
//		edges[1].x += edges[1].dx;
//		y++;
//	}
//	while (y <= edges[1].yEnd)
//	{
//		DrawScanLine(y, (int)edges[1].x, (int)edges[2].x, color);
//		edges[1].x += edges[1].dx;
//		edges[2].x += edges[2].dx;
//		y++;
//	}
//}

