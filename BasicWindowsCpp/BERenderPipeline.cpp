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

void BERenderPipeline::DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color)
{
	Vector3* v[3];
	v[0] = &v1;
	v[1] = &v2;
	v[2] = &v3;

	// sort - To Do - rethink this
	Vector3* tmp;
	if (v[0]->y > v[1]->y) { tmp = v[0]; v[0] = v[1]; v[1] = tmp; }
	if (v[1]->y > v[2]->y) { tmp = v[1]; v[1] = v[2]; v[2] = tmp; }
	if (v[0]->y > v[1]->y) { tmp = v[0]; v[0] = v[1]; v[1] = tmp; }

	Edge edges[3];
	InitEdge(edges,   v[0], v[1]);
	InitEdge(edges+1, v[0], v[2]);
	InitEdge(edges+2, v[1], v[2]);

	unsigned int y = (int)v[0]->y;

	if (y == edges[0].yEnd) // special case of the first edge being flat
	{
		DrawScanLine(y, (int)edges[0].x, (int)edges[0].dx, color);
		y++;
	}
	else while (y <= edges[0].yEnd)
	{
		DrawScanLine(y, (int)edges[0].x, (int)edges[1].x, color);
		edges[0].x += edges[0].dx;
		edges[1].x += edges[1].dx;
		y++;
	}
	while (y <= edges[1].yEnd)
	{
		DrawScanLine(y, (int)edges[1].x, (int)edges[2].x, color);
		edges[1].x += edges[1].dx;
		edges[2].x += edges[2].dx;
		y++;
	}
}

