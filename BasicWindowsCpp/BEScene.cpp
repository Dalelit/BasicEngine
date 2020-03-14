#include "pch.h"
#include "BEScene.h"
#include "BELogger.h"
#include "Submodules/imgui/imgui.h"

BEScene::BEScene()
{
}

BEScene::~BEScene()
{
	Clear();
}

void BEScene::Update(float deltaTime)
{
	for (BEModel* m : models)
	{
		m->Update(deltaTime);
	}

	for (BEPointLight* light : lights)
	{
		light->Update(deltaTime);
	}
}

void BEScene::Clear()
{
	for (auto m : models) delete m;
	models.clear();
	for (auto t : textures) delete t;
	textures.clear();
	for (auto s : samplers) delete s;
	samplers.clear();
	for (auto l : lights) delete l;
	lights.clear();
}

void BEScene::ShowImguiEnvironment(bool* pOpen)
{
	ImGui::Begin("Lights", pOpen);

	if (ImGui::TreeNode("Ambient light"))
	{
		ImGui::ColorEdit4("Color ", ambientLight.color.m128_f32);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Direcitonal light"))
	{
		ImGui::DragFloat3("Direction ", directionalLight.direction.m128_f32, 0.01f, 0.0f, 1.0f);
		ImGui::ColorEdit4("Color ", directionalLight.color.m128_f32);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Point lights"))
	{
		for (int i = 0; i < lights.size(); i++)
		{
			std::string title = "Point light " + std::to_string(i);

			if (ImGui::TreeNode(title.c_str()))
			{
				ImGui::DragFloat3("Position ", lights[i]->position.m128_f32);
				ImGui::ColorEdit4("Color ", lights[i]->color.m128_f32);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void BEScene::ShowImguiEntities(bool* pOpen)
{
	ImGui::Begin("Entities", pOpen);

	for (auto& model : models) model->ShowImguiTreeNode();

	ImGui::End();
}

