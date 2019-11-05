#pragma once
#include "BEWorld.h"
#include "BECamera.h"
#include "BECanvas.h"

using namespace DirectX;
using namespace SimpleMath;

// TO DO - Max to heap not stack - Urgent?
#define BERENDERPIPELINE_MAX_EDGES    200
#define BERENDERPIPELINE_MAX_TRIEDGES 100
#define BERENDERPIPELINE_MAX_VERTICES 200

class BERenderPipeline
{
public:
	struct BEEdge
	{
		unsigned int y;
		unsigned int yEnd;
		float x;
		float dx;
		float z;
		float dz;
		Color c;
		Color dc;
	};

	struct BETriEdge
	{
		unsigned int yStart;
		BEEdge* e0;
		BEEdge* e1;
		BETriEdge* next;
	};

	BEWorld* pWorld;
	BECamera* pCamera;
	BECanvas* pCanvas;

	BERenderPipeline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);

	void UpdateScreenSpace();
	void Draw();
	void DrawV1();

	void DrawModel(BEModel* pModel);

	void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color);

private:
	// preallocated memory for the number crunching
	BEEdge edges[BERENDERPIPELINE_MAX_EDGES] = {};
	BETriEdge triedges[BERENDERPIPELINE_MAX_TRIEDGES] = {};
	Vector3 screenSpaceVerticies[BERENDERPIPELINE_MAX_VERTICES] = {};

	// Ensure vFrom is lowest y
	inline void InitEdge(BEEdge* e, Vector3* vFrom, Vector3* vTo, Color cFrom, Color cTo)
	{
		e->yEnd = (int)vTo->y;
		e->x = vFrom->x;
		e->z = vFrom->z;
		e->c = cFrom;
		float yDiff = vTo->y - vFrom->y;
		if (yDiff != 0) // check that not parallel
		{
			e->dx = (vTo->x - vFrom->x) / yDiff;
			e->dz = (vTo->z - vFrom->z) / yDiff;
			e->dc = (cTo - cFrom) / yDiff;
		}
		//else // otherwise... ignore for now as not using
		//{
		//	e->dx = vTo->x;
		//	e->dz = vTo->z;
		//}
	}

	inline void UpdateEdge(BEEdge* e)
	{
		e->x += e->dx;
		e->z += e->dz;
	}

	inline void DrawScanLine(unsigned int y, unsigned int x1, unsigned int x2, Color color)
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
};

