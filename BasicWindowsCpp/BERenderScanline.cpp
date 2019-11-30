#include "BERenderPipeline.h"

// To Do
// - scan line has issues as the verticies, scan line keeps running past the edge :-(

BERenderPipelineScanline::BERenderPipelineScanline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pScene = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;

	// pre allocate memory
	edges = new BEEdge[BERENDERPIPELINE_MAX_EDGES];
	triedges = new BETriEdge[BERENDERPIPELINE_MAX_TRIEDGES];
	screenSpaceVerticies = new XMVECTOR[BERENDERPIPELINE_MAX_VERTICES];
}

BERenderPipelineScanline::~BERenderPipelineScanline()
{
	delete edges;
	delete triedges;
	delete screenSpaceVerticies;
}

// Ensure vFrom is lowest y
inline void BERenderPipelineScanline::InitEdge(BEEdge* e, XMVECTOR* vFrom, XMVECTOR* vTo, XMVECTOR cFrom, XMVECTOR cTo)
{
	e->yEnd = (int)XMVectorGetY(*vTo);  // vTo->y;
	e->x = XMVectorGetX(*vFrom); // vFrom->x;
	e->z = XMVectorGetZ(*vFrom); // vFrom->z;
	e->c = cFrom;
	float yDiff = XMVectorGetY(*vTo) - XMVectorGetY(*vFrom); // vTo->y - vFrom->y;
	if (yDiff != 0) // check that not parallel
	{
		e->dx = (XMVectorGetX(*vTo) - XMVectorGetX(*vFrom)) / yDiff;
		e->dz = (XMVectorGetZ(*vTo) - XMVectorGetZ(*vFrom)) / yDiff;
		e->dc = (cTo - cFrom) / yDiff;
	}
}

inline void BERenderPipelineScanline::UpdateEdge(BEEdge* e)
{
	e->x += e->dx;
	e->z += e->dz;
	e->c += e->dc;
}

inline void BERenderPipelineScanline::DrawScanLine(unsigned int y, unsigned int x1, unsigned int x2, XMVECTOR color)
{
	unsigned int line = y * pCanvas->width;

	unsigned int x, xt;
	if (x1 < x2) { x = x1; xt = x2; }
	else { x = x2; xt = x1; }

	if (x < 0) x = 0;

	while (x < pCanvas->width && x <= xt)
	{
		pCanvas->buffer[line + x] = color;
		x++;
	}
}

// To Do:
// - edge color has artifacts
//
void BERenderPipelineScanline::Draw()
{
	// loop through each entity/mesh/triangle
	//   convert to screen space
	//   if inside screen space, then create edges in pixel space
	// draw edges

	// To Do - break up the function?

	BETriEdge* sortedList = NULL;

	// create sorted list of triangle-edges
	{
		BEEdge* edge = edges;
		BETriEdge* triedage = triedges;

		for (unsigned int eindx = 0; eindx < pScene->entityCount; eindx++) // for each entity
		{
			BEMesh* m = pScene->entities[eindx]->mesh; // get it's mesh

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
				}

				unsigned int tindx = 0;
				unsigned int trinum = 0;

				while (tindx < m->tBufferSize) // look at each triangle
				{
					XMVECTOR normal = m->normals[trinum];

					XMVECTOR v0 = screenSpaceVerticies[m->triangles[tindx++]];
					XMVECTOR v1 = screenSpaceVerticies[m->triangles[tindx++]];
					XMVECTOR v2 = screenSpaceVerticies[m->triangles[tindx++]];

					// check if the triangle is facing the same direction as the camera, then it is facing away so cull
					//if (v0.Dot(normal) < 0.0f)
					//{
					//	tindx += 3;
					//	trinum++;
					//	break;
					//}

					//if (pCamera->Dot(normal) > 0.0f)
					//{
					//	tindx += 3;
					//	trinum++;
					//	break;
					//}

					// sort out lighiting value
					XMVECTOR lights = { 0,0,0,0 };
					for (unsigned int lindx = 0; lindx < pScene->lightCount; lindx++)
					{
						lights += pScene->lights[lindx]->CalculateColor(normal);
					}

					lights = lights / (float)pScene->lightCount;

					//Color c = 0.5f * ambient + 0.5f * lights;
					//c.Saturate();


					XMVECTOR c0, c1, c2;

					//if (m->colors)
					//{
					//	// To do
					//	c0 = { 1,0,0 };
					//	c1 = { 0,1,0 };
					//	c2 = { 0,0,1 };
					//}
					//else
					{
						c0 = XMVectorSaturate( pScene->entities[eindx]->color * 0.5f + lights * 0.5f );
						c1 = c0;
						c2 = c0;
					}

					// check it's in the screen bounds
					if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1) || pCamera->OveralpsScreen(v2))
					{
						// convert to pixel coords
						v0 = pCanvas->ScreenToPixel(v0);
						v1 = pCanvas->ScreenToPixel(v1);
						v2 = pCanvas->ScreenToPixel(v2);

						// order the verticies lowest to highest // To do - is there something more efficient?
						if (XMVectorGetY(v0) > XMVectorGetY(v1)) SWAPXMVECTOR(v0, v1) // v0.y > v1.y
						{
							if (XMVectorGetY(v1) > XMVectorGetY(v2)) // v1.y > v2.y
							{
								SWAPXMVECTOR(v1, v2)
									if (XMVectorGetY(v0) > XMVectorGetY(v1)) // v0.y > v1.y
										SWAPXMVECTOR(v0, v1)
							}
						}

						// create the edges and triedge
						InitEdge(edge, &v0, &v1, c0, c1);
						triedage->e0 = edge;
						triedage->yStart = (unsigned int)XMVectorGetY(v0); // v0.y;
						edge++;
						InitEdge(edge, &v0, &v2, c0, c2);
						triedage->e1 = edge;
						edge++;
						InitEdge(edge, &v1, &v2, c1, c2);
						edge++;

						// special case where first edge e0 is flat... replace it with e2
						// scan lines will then both stop for e0, e1 at the same time
						//if (v0.y == v1.y) triedage->e0 += 2;
						if (XMVectorGetY(v0) == XMVectorGetY(v1)) triedage->e0 += 2;

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
					else int dummybp = 0;

					trinum++;
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
		int y = activeList->yStart;
		int line = y * pCanvas->width;

		while ((sortedList || activeList) && y < (int)pCanvas->height) // To Do: not cast to (int) all the time?
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
					// To Do: the floats are converted to int... should there be something to align the x,y,z,color?
					//        e.g. reclaulte the y=1.1 becoming y=1... so move the x,z,color by -0.1 first
					//        Haven't thought this one through

					int x = (int)current->e0->x;
					int xt = (int)current->e1->x;

					float z = current->e0->z;
					float dz = 0.0f;

					XMVECTOR c = current->e0->c;
					XMVECTOR dc = {};

					if (x != xt) // not a single x step
					{
						float dx = current->e1->x - current->e0->x;
						dz = (current->e1->z - current->e0->z) / dx;
						dc = (current->e1->c - current->e0->c) / dx;
					}

					if (xt < x)
					{
						SWAPINT(x, xt);

						z = current->e1->z;
						dz = -dz;

						c = current->e1->c;
						dc = -dc;
					}

					if (x < 0) x = 0; // move onto the screen if off it

					while (x <= xt && x < (int)pCanvas->width) // To Do: not cast to (int) all the time?
					{
						float* depthBuffer = pCanvas->depthBuffer + y * pCanvas->width + x; // to do: clean this up.. and think about int types
						float depth = *depthBuffer;

						if (z < depth) // closer so draw it
						{
							pCanvas->buffer[line + x] = c;
							*depthBuffer = z; // update the depth buffer
						}

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
