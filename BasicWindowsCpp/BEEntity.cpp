#include "BEEntity.h"

using namespace DirectX;

BEEntity::BEEntity(XMFLOAT3A _position)
{
	position = XMLoadFloat3A(&_position);
	position.m128_f32[3] = 1.0f;
}
