#pragma once
#include "BEDirectXResourceManager.h"
#include "BEDirectXConstantBuffer.h"
#include "BEDXVertexBuffer.h"
#include "BEModel.h"
#include <memory>

//////
// Scene
//  --> Model
//     --> Mesh
//     -----> Entity


class BEDXEntity
{
public:
	BEDXEntity(BEDirectXResourceManager& resourceMgr);

	void Update(BEDirectXDevice& device, BEEntity& entity);
	void Bind(BEDirectXDevice& device);

	std::unique_ptr<BEDirectXEntityConstantBuffer> entityVSCB;
	std::unique_ptr<BEDirectXEntityConstantBuffer> entityPSCB;
};

class BEDXMesh
{
public:
	BEDXMesh(BEDirectXResourceManager& resourceMgr, BEMesh* pMesh);

	void Update() {}; // do nothing for now.
	void Bind(BEDirectXDevice& device);
	void Draw(BEDirectXDevice& device);

	BEMesh* pMesh = nullptr;

	std::unique_ptr<BEDXVertexBuffer> vertexBuffer;
	std::vector<BEDirectXResource*> resources;
};

class BEDXModel
{
public:
	BEDXModel(BEDirectXResourceManager& resourceMgr, BEModel* pModel);

	void Bind(BEDirectXDevice& device);
	void Draw(BEDirectXDevice& device);

	BEModel* pModel = nullptr;

	std::vector<BEDXMesh> meshes;
	BEDXEntity entityCB;
};

class BEDXScene
{
public:
	void LoadScene(BEDirectXResourceManager& resourceMgr, BEScene* pScene, BECamera* pCamera);
	void Bind(BEDirectXDevice& device);
	void Draw(BEDirectXDevice& device);

	std::unique_ptr<BEDirectXConstantBuffer> sceneVSCB;
	std::unique_ptr<BEDirectXConstantBuffer> scenePSCB;

	std::vector<BEDXModel> models;
};

