#include "BEDXScene.h"
#include "BEDXVSConstantBuffer.h"
#include "BEDXPSConstantBuffer.h"
#include "BEDXEntityVSConstantBuffer.h"
#include "BEDXEntityPSConstantBuffer.h"
#include "BEDXVertexBuffer.h"
#include "BEDXVertexShader.h"
#include "BEDXPixelShader.h"
#include "BEDXTexture.h"


void BEDXScene::LoadScene(BEDirectXResourceManager& resourceMgr, BEScene* pScene, BECamera* pCamera)
{
	assert(pScene); assert(pCamera);

	// to do: clear our previous versions of stuff when scene already loaded.
	
	sceneVSCB = std::make_unique<BEDXVSConstantBuffer>(resourceMgr.device, pScene, pCamera);
	scenePSCB = std::make_unique<BEDXPSConstantBuffer>(resourceMgr.device, pScene, pCamera);

	sceneVSCB->slot = 0u;
	scenePSCB->slot = 0u;

	for (BEModel* pModel : pScene->models)
	{
		models.emplace_back(resourceMgr, pModel);
	}
}

void BEDXScene::Bind(BEDirectXDevice& device)
{
	sceneVSCB->Bind(device);
	scenePSCB->Bind(device);
}

void BEDXScene::Draw(BEDirectXDevice& device)
{
	Bind(device);

	for (auto& model : models) model.Draw(device);
}

void BEDXScene::Clear()
{
	sceneVSCB = nullptr;
	scenePSCB = nullptr;
	models.clear();
}

BEDXModel::BEDXModel(BEDirectXResourceManager& resourceMgr, BEModel* pModel) :
	pModel(pModel),
	entityCB(resourceMgr)
{
	assert(pModel);

	meshes.emplace_back(resourceMgr, pModel->pMesh);
}

void BEDXModel::Bind(BEDirectXDevice& device)
{
	// nothing to do for now
}

void BEDXModel::Draw(BEDirectXDevice& device)
{
	Bind(device);

	for (auto& mesh : meshes)
	{
		mesh.Bind(device);

		for (auto& entity : pModel->entities)
		{
			entityCB.Update(device, entity);
			entityCB.Bind(device);
			mesh.Draw(device);
		}
	}
}

BEDXEntity::BEDXEntity(BEDirectXResourceManager& resourceMgr)
{
	entityVSCB = std::make_unique<BEDXEntityVSConstantBuffer>(resourceMgr.device);
	entityPSCB = std::make_unique<BEDXEntityPSConstantBuffer>(resourceMgr.device);

	entityVSCB->slot = 1u;
	entityPSCB->slot = 1u;
}

void BEDXEntity::Update(BEDirectXDevice& device, BEEntity& entity)
{
	entityVSCB->Update(device, entity);
	entityPSCB->Update(device, entity);
}

void BEDXEntity::Bind(BEDirectXDevice& device)
{
	entityVSCB->Bind(device);
	entityPSCB->Bind(device);
}

BEDXMesh::BEDXMesh(BEDirectXResourceManager& resourceMgr, BEMesh* pMesh) :
	pMesh(pMesh)
{
	assert(pMesh);

	if (pMesh->material.IsTextured())
	{
		resources.push_back(resourceMgr.AccessResource<BEDXVertexShaderPosNorColTex>(resourceMgr.device, L"VertexShaderTex.cso"));
		resources.push_back(resourceMgr.AccessResource<BEDXPixelShader>(resourceMgr.device, L"PixelShaderTex.cso"));
		resources.push_back(resourceMgr.AccessResource<BEDXTexture>(resourceMgr.device, pMesh->material.pTextureSampler->texture));
	}
	else
	{
		resources.push_back(resourceMgr.AccessResource<BEDXVertexShaderPosNorColTex>(resourceMgr.device, L"VertexShader.cso"));
		resources.push_back(resourceMgr.AccessResource<BEDXPixelShader>(resourceMgr.device, L"PixelShader.cso"));
	}

	if (pMesh->topology == BEMesh::BEMeshTopology::TRIANGLE_INDEX)
		vertexBuffer = std::make_unique<BEDXVertexBufferIndexed>(resourceMgr.device, pMesh);
	else
		vertexBuffer = std::make_unique<BEDXVertexBuffer>(resourceMgr.device, pMesh);

}

void BEDXMesh::Bind(BEDirectXDevice& device)
{
	for (auto& r : resources) r->Bind(device);
	vertexBuffer->Bind(device);
}

void BEDXMesh::Draw(BEDirectXDevice& device)
{
	vertexBuffer->Draw(device);
}
