#include "pch.h"
#include "BEModel.h"
#include "Submodules/imgui/imgui.h"

unsigned int BEModel::modelCount = 0;

BEModel::BEModel() : BEModel("Model" + std::to_string(modelCount))
{
}

BEModel::BEModel(std::string name) : name(name)
{
	modelCount++;
}

BEModel::~BEModel()
{
	delete pMesh;
}

void BEModel::Update(float deltaTime)
{
	// do the component updates first
	for (auto func : updateFunctions) (*func)(this, deltaTime);

	for (BEEntity& entity : entities)
	{
		if (entity.active) // to do: move inactive out of this list?
		{
			entity.Update(deltaTime);
		}
	}

}

// to do: not going to be efficient to resize vector. Not explecting to use this much in the future so not changing for now.
BEEntity* BEModel::CreateInstance(DirectX::XMFLOAT3A position)
{
	entities.emplace_back(BEEntity(position));

	BEEntity* e = &entities.back();
	e->pMaterial = &this->pMesh->material;

	return e;
}

void BEModel::CreateBulkInstances(unsigned int amount)
{
	unsigned int requiredCapacity = (unsigned int)entities.size() + amount;

	if (entities.capacity() < requiredCapacity) ReserveInstances(requiredCapacity);

	for (unsigned int i = 0; i < amount; i++)
	{
		entities.emplace_back(BEEntity());
		entities.back().pMaterial = &this->pMesh->material;
	}
}

void BEModel::ReserveInstances(unsigned int amount)
{
	// to do: logging when this happens?
	entities.reserve(amount);
}

void BEModel::AddPhysics()
{
	physicsData.reserve(entities.capacity());

	for (unsigned int i = (unsigned int)physicsData.size(); i < entities.size(); i++)
	{
		physicsData.emplace_back(BEComponentPhysics());
	}
}

void BEModel::ShowImguiTreeNode()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		if (pMesh)
		{
			std::string text = "Mesh: " + pMesh->name;
			ImGui::Text(text.c_str());
		}

		if (materials.size() > 0 && ImGui::TreeNode("Materials"))
		{
			std::string text = "Number of materials " + std::to_string(materials.size());
			ImGui::Text(text.c_str());

			for (auto& mat : materials) mat.ShowImguiTreeNode();
			
			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Entites"))
		{
			std::string text = "Number of entities " + std::to_string(entities.size());
			ImGui::Text(text.c_str());

			for (auto& ent : entities) ent.ShowImguiTreeNode();

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
