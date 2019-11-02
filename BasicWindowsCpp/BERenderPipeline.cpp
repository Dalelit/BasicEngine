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
				if (pv1->z > 0.0f && pv2->z > 0.0f)
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

