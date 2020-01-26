#include "BEEntity.h"

using namespace DirectX;

BEEntity::BEEntity(XMFLOAT3A _position)
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

BEEntityComponent::BEEntityComponent(BEEntity* pEntity) :
	pEntity(pEntity)
{
}
