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
		parent->Translate((nx - x), 0.0f, (nz - z));
		x = nx;
		z = nz;
	}

private:
	float current;
	float x;
	float z;
};

class BEEntityComponentSpinAroundY : public BEEntityComponent
{
public:
	float speed = 3.0f;

	BEEntityComponentSpinAroundY(BEEntity* pEntity) :
		BEEntityComponent(pEntity)
	{}

	void Update(float deltatime) {
		parent->Rotate(0.0f, deltatime * speed, 0.0f);
	}

private:
	float current = 0.0f;
};
