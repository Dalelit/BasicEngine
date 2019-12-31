#pragma once
#include "BEEntity.h"

class BEEntityComponentOrbit : public BEEntityComponent
{
public:
	float speed = 3.0f;
	float radius = 1.0f;

	BEEntityComponentOrbit(BEEntity* pEntity) :
		BEEntityComponent(pEntity)
	{}

	void Update(float deltatime) {
		current += deltatime;
		float nx = radius * cosf(current * speed);
		float nz = radius * sinf(current * speed);
		parent->position.m128_f32[0] += (nx - x);
		parent->position.m128_f32[2] += (nz - z);
		x = nx;
		z = nz;
	}

private:
	float current;
	float x;
	float z;
};
