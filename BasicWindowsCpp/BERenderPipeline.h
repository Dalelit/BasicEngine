#pragma once
#include "BEWorld.h"
#include "BECamera.h"
#include "BECanvas.h"

using namespace DirectX;
using namespace SimpleMath;

class BERenderPipeline
{
public:
	BEWorld* pWorld;
	BECamera* pCamera;
	BECanvas* pCanvas;

	BERenderPipeline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);

	void UpdateScreenSpace();
	void Draw();

	void DrawModel(BEModel* pModel);

	void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color);

	struct Edge
	{
		unsigned int yEnd;
		float x;
		float dx;
	};

	// Ensure vFrom is lowest y
	inline void InitEdge(Edge* e, Vector3* vFrom, Vector3* vTo)
	{
		e->yEnd = (int)vTo->y;
		e->x = vFrom->x;
		float yDiff = (vTo->y - vFrom->y);
		if (yDiff == 0) e->dx = vTo->x; // if parallel set dx to the target x
		else e->dx = (vTo->x - vFrom->x) / yDiff;
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

