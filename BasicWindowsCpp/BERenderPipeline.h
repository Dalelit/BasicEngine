#pragma once
#include "BEWorld.h"
#include "BECamera.h"
#include "BECanvas.h"

// Implementations so far...
//
// BERenderPipelineScanline
// BERenderPipelineRaytrace
// BERenderPipelineWireframe


// TO DO - sensible sizes for memory buffers?
#define BERENDERPIPELINE_MAX_EDGES    20000
#define BERENDERPIPELINE_MAX_TRIEDGES 10000
#define BERENDERPIPELINE_MAX_VERTICES 20000


// Base virtual class
//
class BERenderPipeline
{
public:
	BEWorld* pWorld = NULL;
	BECamera* pCamera = NULL;
	BECanvas* pCanvas = NULL;

	// for slower renderers - looking at you raytracer
	bool exitLoop = false;    // used to exit the rendering loops early
	bool restartLoop = false; // used to force the loop to exit and restart
	bool showBuffer = false;  // used to tell the main thread to show the back buffer for progressive results

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
		XMVECTOR c;
		XMVECTOR dc;
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
	XMVECTOR* screenSpaceVerticies;

	inline void InitEdge(BEEdge* e, XMVECTOR* vFrom, XMVECTOR* vTo, XMVECTOR cFrom, XMVECTOR cTo);
	inline void UpdateEdge(BEEdge* e);
	inline void DrawScanLine(unsigned int y, unsigned int x1, unsigned int x2, XMVECTOR color);
};

//////////////////////////////////////////////////////
// Raytrace
//
class BERenderPipelineRaytrace : public BERenderPipeline
{
public:

	unsigned int raysToProcess;
	unsigned int raysProcessed;

	BERenderPipelineRaytrace(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineRaytrace();

	void Draw();
	void DrawByLine();
	void DrawBySampling();

private:
	void InnerLoop(float px, float py, unsigned int x, unsigned int y, unsigned int line);
	inline void InnerLoop(float px, float py, unsigned int x, unsigned int y) { InnerLoop(px, py, x, y, y * pCanvas->width); };
};

//////////////////////////////////////////////////////
// Wireframe
//
class BERenderPipelineWireframe : public BERenderPipeline
{
public:
	bool drawNormals = true;
	XMVECTOR normalColor = { 0.5f, 0.5f, 0.5f, 1.0f};

	bool backfaceCull = true;

	BERenderPipelineWireframe(BEWorld* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineWireframe();

	void Draw();

private:
	// preallocated memory for the number crunching
	XMVECTOR* screenSpaceVerticies;
};

