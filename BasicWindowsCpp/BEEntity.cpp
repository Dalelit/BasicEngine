#include "pch.h"
#include "BEEntity.h"
#include "Submodules/imgui/imgui.h"

using namespace DirectX;

BEEntity::BEEntity()
{
	static unsigned int entityCounter;

	id = entityCounter;
	entityCounter++;
}

BEEntity::BEEntity(XMFLOAT3A _position) : BEEntity()
{
	position = XMLoadFloat3A(&_position);
	position.m128_f32[3] = 1.0f;
}

BEEntity::~BEEntity()
{
	for (auto e : children) delete e;
	for (auto c : components) delete c;
}

void BEEntity::Update(float deltaTime)
{
	// initialise the tranform matricies
	if (parent) // to either the parent one
	{
		transform = parent->transform;
		transformRotation = parent->transformRotation;
	}
	else // or the identity
	{
		// to do: think this through more
		transform = DirectX::XMMatrixIdentity();
		transformRotation = DirectX::XMMatrixIdentity();
	}

	// update the tranform matricies

	transformRotation = XMMatrixScalingFromVector(scale) * XMMatrixRotationRollPitchYawFromVector(rotation);
	transform *= transformRotation;
	transform *= XMMatrixTranslationFromVector(position);

	invTransform = XMMatrixInverse(nullptr, transform);
	invTransformRotation = XMMatrixInverse(nullptr, transformRotation);

	// loop throught the components... they may update the matricies further
	for (BEEntityComponent* cmp : components) cmp->Update(deltaTime);

	// update the children entities
	for (auto e : children) e->Update(deltaTime);
}

void BEEntity::ShowImguiTreeNode()
{
	std::string name = "Entity" + std::to_string(id);
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::Checkbox("Active", &active);
		ImGui::DragFloat4("Scale", scale.m128_f32);
		ImGui::DragFloat4("Position", position.m128_f32);
		ImGui::DragFloat4("Rotation", rotation.m128_f32);

		if (pMaterial) pMaterial->ShowImguiTreeNode();

		if (components.size() > 0)
		{
			std::string text = "Components " + std::to_string(components.size());
			ImGui::Text(text.c_str());
		}

		if (parent) ImGui::Text(("Has parent " + std::to_string(parent->id)).c_str());
		if (children.size()) ImGui::Text(("Has children " + std::to_string(children.size())).c_str());

		ImGui::TreePop();
	}
}

BEEntityComponent::BEEntityComponent(BEEntity* pEntity) :
	pEntity(pEntity)
{
}
