///////////////////////////////////////////
// Depricated as it is now replaced.     //
// Keeping for it here for the memories. //
///////////////////////////////////////////

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
	BEScene* pScene = nullptr;
	BECamera* pCamera = nullptr;
	BECanvas* pCanvas = nullptr;

	// for slower renderers - looking at you raytracer
	bool exitLoop = false;    // used to exit the rendering loops early
	bool restartLoop = false; // used to force the loop to exit and restart
	bool showBuffer = false;  // used to tell the main thread to show the back buffer for progressive results

	//BERenderPipeline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas)
	//{
	//	pScene = _pScene;
	//	pCamera = _pCamera;
	//	pCanvas = _pCanvas;
	//}

	virtual void Draw() = 0;
	virtual void Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height) = 0;
	virtual void ResetStats() {};
};

//////////////////////////////////////////////////////
// Scan line
//
class BERenderPipelineScanline : public BERenderPipeline
{
public:
	BERenderPipelineScanline(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineScanline();

	void Draw();
	void Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height) { throw "Not implemented yet"; }

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
		DirectX::XMVECTOR c;
		DirectX::XMVECTOR dc;
	};

	struct BETriEdge
	{
		int yStart;
		BEEdge* e0;
		BEEdge* e1;
		BETriEdge* next;
		BEMesh* mesh;
	};

	// preallocated memory for the number crunching
	BEEdge* edges;
	BETriEdge* triedges;
	BEVertex* screenSpaceVerticies;

	inline void InitEdge(BEEdge* e, DirectX::XMVECTOR* vFrom, DirectX::XMVECTOR* vTo, DirectX::XMVECTOR cFrom, DirectX::XMVECTOR cTo, DirectX::XMFLOAT2 tcFrom, DirectX::XMFLOAT2 tcTo);
	inline void UpdateEdge(BEEdge* e);
	inline void DrawScanLine(unsigned int y, unsigned int x1, unsigned int x2, DirectX::XMVECTOR color);
};


//////////////////////////////////////////////////////
// Wireframe
//
class BERenderPipelineWireframe : public BERenderPipeline
{
public:
	DirectX::XMVECTOR lineColor = { 1.0f, 1.0f, 1.0f };

	bool backfaceCull = false;
	DirectX::XMVECTOR backlineColor = { 0.25f, 0.25f, 0.25f };

	bool drawNormals = false;
	float normalLength = 0.5f;
	DirectX::XMVECTOR normalColor = { 0.5f, 0.3f, 0.3f };

	BERenderPipelineWireframe(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas);
	~BERenderPipelineWireframe();

	void Draw();
	void Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height) { throw "Not implemented yet"; }

private:
	struct BELineInfo
	{
		DirectX::XMVECTOR v0;
		DirectX::XMVECTOR v1;
		DirectX::XMVECTOR v2;
		DirectX::XMVECTOR color;
	};

	// preallocated memory for the number crunching
	BEVertex* screenSpaceVerticies;
	BELineInfo* linesToDraw;
};

