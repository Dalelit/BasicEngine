#pragma once
#include "BEWorld.h"
#include "BECamera.h"
#include "BECanvas.h"

using namespace DirectX;
using namespace SimpleMath;

// Implementations so far...
//
// BERenderPipelineScanline
// BERenderPipelineRaytrace
// BERenderPipelineWireframe


// TO DO - sensible sizes for memory buffers?
#define BERENDERPIPELINE_MAX_EDGES    20000
#define BERENDERPIPELINE_MAX_TRIEDGES 10000
#define BERENDERPIPELINE_MAX_VERTICES 20000


// Base virtual clase
//
class BERenderPipeline
{
public:
	BEWorld* pWorld = NULL;
	BECamera* pCamera = NULL;
	BECanvas* pCanvas = NULL;
	bool exitLoop = false; // used to exit the rendering loops early... mainly for slower renderers

	//BERenderPipeline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas)
	//{
	//	pWorld = _pWorld;
	//	pCamera = _pCamera;
	//	pCanvas = _pCanvas;
	//}

	virtual void Draw() = 0;
};

//////////////////////////////////////////////////////
// Scan line
//
class BERenderPipelineScanline : public BERenderPipeline
{
public:
	BERenderPipelineScanline(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineScanline();

	void Draw();

private:
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

	// preallocated memory for the number crunching
	BEEdge* edges;
	BETriEdge* triedges;
	Vector3* screenSpaceVerticies;

	inline void InitEdge(BEEdge* e, Vector3* vFrom, Vector3* vTo, Color cFrom, Color cTo);
	inline void UpdateEdge(BEEdge* e);
	inline void DrawScanLine(unsigned int y, unsigned int x1, unsigned int x2, Color color);
};

//////////////////////////////////////////////////////
// Raytrace
//
class BERenderPipelineRaytrace : public BERenderPipeline
{
public:
	BERenderPipelineRaytrace(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineRaytrace();

	void Draw();
};

//////////////////////////////////////////////////////
// Wireframe
//
class BERenderPipelineWireframe : public BERenderPipeline
{
public:
	BERenderPipelineWireframe(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineWireframe();

	void Draw();

private:
	// preallocated memory for the number crunching
	Vector3* screenSpaceVerticies;
};

