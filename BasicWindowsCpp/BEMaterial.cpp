#include "pch.h"
#include "BEMaterial.h"
#include "Submodules/imgui/imgui.h"

using namespace DirectX;

unsigned int BEMaterial::materialCount = 0;

BEMaterial::BEMaterial(std::string name) : name(name)
{
	materialCount++;
}

BEMaterial::BEMaterial() : BEMaterial("Material" + std::to_string(materialCount))
{
}

BEMaterial& BEMaterial::Randomise(bool normalizeColor)
{
	std::random_device dev;
	std::default_random_engine gen(dev());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	auto rng = std::bind(dis, gen);

	auto randomVector = [&]() -> XMVECTOR {
		XMVECTOR v = { rng(), rng(), rng(), 1.0f };
		if (normalizeColor) v = XMVector3Normalize(v);
		v.m128_f32[3] = 1.0f;
		return v;
	};

	ambientColor = randomVector();
	diffuseColor = randomVector();
	specularColor = randomVector();
	emmissionColor = randomVector();
	specularExponent = rng() * 100.0f;

	return *this;
}

void BEMaterial::ShowImguiTreeNode()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::ColorEdit3("Ambient", ambientColor.m128_f32);
		ImGui::ColorEdit3("Diffuse", diffuseColor.m128_f32);
		ImGui::ColorEdit3("Specular", specularColor.m128_f32);
		ImGui::DragFloat("Specular power", &specularExponent);
		
		if (IsTextured()) ImGui::Text(("Is textured: " + pTextureSampler->texture.name).c_str());
		else ImGui::Text("Is not textured");

		ImGui::TreePop();
	}
}
