#pragma once
#include "BEWorld.h"
#include "BECamera.h"
#include "BECanvas.h"

using namespace DirectX;
using namespace SimpleMath;

// TO DO - sensible sizes?
#define BERENDERPIPELINE_MAX_EDGES    20000
#define BERENDERPIPELINE_MAX_TRIEDGES 10000
#define BERENDERPIPELINE_MAX_VERTICES 20000

class BERenderPipeline
{
public:
	struct BEEdge
	{
		int y;
		int yEnd;
		float x;
		float dx;
		float z;
		float dz;
		Color c;
		Color dc;
	};

	struct BETriEdge
	{
		int yStart;
		BEEdge* e0;
		BEEdge* e1;
		BETriEdge* next;
	};

	BEWorld* pWorld;
	BECamera* pCamera;
	BECanvas* pCanvas;

	BERenderPipeline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipeline();

	void ScanLine();
	void Raytrace();
	void WireFrame();

	// to delete
	//void UpdateScreenSpace();
	//void DrawV1();
	//void DrawModel(BEModel* pModel);
	//void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color);

private:
	// preallocated memory for the number crunching
	BEEdge* edges;
	BETriEdge* triedges;
	Vector3* screenSpaceVerticies;

	inline void InitEdge(BEEdge* e, Vector3* vFrom, Vector3* vTo, Color cFrom, Color cTo);
	inline void UpdateEdge(BEEdge* e);
	inline void DrawScanLine(unsigned int y, unsigned int x1, unsigned int x2, Color color);
};

