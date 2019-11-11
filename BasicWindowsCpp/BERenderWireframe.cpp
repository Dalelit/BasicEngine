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
	screenSpaceVerticies = new Vector3[BERENDERPIPELINE_MAX_VERTICES];
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

			while (tindx < m->tBufferSize) // look at each triangle
			{
				Vector3 v0 = screenSpaceVerticies[m->triangles[tindx++]];
				Vector3 v1 = screenSpaceVerticies[m->triangles[tindx++]];
				Vector3 v2 = screenSpaceVerticies[m->triangles[tindx++]];

				Color c = pWorld->entities[eindx]->color;

				// check it's in the screen bounds
				if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1) || pCamera->OveralpsScreen(v2))
				{
					pCanvas->DrawLineSafe(v0, v1, c);
					pCanvas->DrawLineSafe(v1, v2, c);
					pCanvas->DrawLineSafe(v2, v0, c);
				}
			}

			unsigned int lindx = 0;
			while (lindx < m->lBufferSize)
			{
				Vector3 v0 = screenSpaceVerticies[m->lines[lindx++]];
				Vector3 v1 = screenSpaceVerticies[m->lines[lindx++]];

				Color c = pWorld->entities[eindx]->color;

				// check it's in the screen bounds
				if (pCamera->OveralpsScreen(v0) || pCamera->OveralpsScreen(v1))
				{
					pCanvas->DrawLineSafe(v0, v1, c);
				}
			}
		}
	}
}
