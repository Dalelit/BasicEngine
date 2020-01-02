#pragma once
#include "BEEntity.h"
#include <random>

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

class BEEntityComponentSpin : public BEEntityComponent
{
public:
	float pitchSpeed = 0.0f;
	float yawSpeed   = 0.0f;
	float rollSpeed  = 0.0f;

	BEEntityComponentSpin(BEEntity* pEntity) :
		BEEntityComponent(pEntity)
	{
		Randomise();
	}

	BEEntityComponentSpin(BEEntity* pEntity, float pitchSpeed, float yawSpeed, float rollSpeed) :
		BEEntityComponent(pEntity),
		pitchSpeed(pitchSpeed),
		yawSpeed(yawSpeed),
		rollSpeed(rollSpeed)
	{
	}

	void Update(float deltatime) {
		parent->Rotate(deltatime * pitchSpeed, deltatime * yawSpeed, deltatime * rollSpeed);
	}

	void Randomise() {
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<float> dist(0.0f, DirectX::XMConvertToRadians(90));

		pitchSpeed = dist(gen);
		yawSpeed = dist(gen);
		rollSpeed = dist(gen);
	}

private:
	float current = 0.0f;

};
