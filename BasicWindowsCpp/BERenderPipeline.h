#pragma once
#include "BEScene.h"
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
	BEScene* pScene = NULL;
	BECamera* pCamera = NULL;
	BECanvas* pCanvas = NULL;

	// for slower renderers - looking at you raytracer
	bool exitLoop = false;    // used to exit the rendering loops early
	bool restartLoop = false; // used to force the loop to exit and restart
	bool showBuffer = false;  // used to tell the main thread to show the back buffer for progressive results

	//BERenderPipeline(BEScene* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas)
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
	BERenderPipelineScanline(BEScene* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
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
		float u;
		float du;
		float v;
		float dv;
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
	BEVertex* screenSpaceVerticies;

	inline void InitEdge(BEEdge* e, XMVECTOR* vFrom, XMVECTOR* vTo, XMVECTOR cFrom, XMVECTOR cTo, XMFLOAT2 tcFrom, XMFLOAT2 tcTo);
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

	bool backfaceCull = true;

	BERenderPipelineRaytrace(BEScene* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineRaytrace();

	void Draw();
	void DrawByLine();
	void DrawBySampling();

	inline bool TriangleIntersects(FXMVECTOR Origin, FXMVECTOR Direction, FXMVECTOR V0, GXMVECTOR V1, HXMVECTOR V2, float& Dist, float& _u, float& _v);

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
	bool drawNormals = false;
	XMVECTOR lineColor = { 1.0f, 1.0f, 1.0f };
	XMVECTOR backlineColor = { 0.25f, 0.25f, 0.25f };
	XMVECTOR normalColor = { 0.5f, 0.5f, 0.5f };

	bool backfaceCull = false;
	//float backfaceColorStrength = 0.25f;

	BERenderPipelineWireframe(BEScene* _pWorld, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineWireframe();

	void Draw();

private:
	struct BELineInfo
	{
		XMVECTOR v0;
		XMVECTOR v1;
		XMVECTOR v2;
		XMVECTOR color;
	};

	// preallocated memory for the number crunching
	BEVertex* screenSpaceVerticies;
	BELineInfo* linesToDraw;
};

