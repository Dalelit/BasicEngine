#include "BEMaterial.h"
#include <random>
#include <functional>

using namespace DirectX;

BEMaterial::BEMaterial() : BEMaterial(L"")
{
}

BEMaterial::BEMaterial(std::wstring name)
{
	id = L"BEMaterial" + name + std::to_wstring(++instanceCounter);
}

BEMaterial& BEMaterial::Randomise()
{
	std::random_device dev;
	std::default_random_engine gen(dev());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	auto rng = std::bind(dis, gen);

	auto randomVector = [](auto& rng) -> XMVECTOR {
		XMVECTOR v = { rng(), rng(), rng(), 1.0f };
		v = XMVector3Normalize(v);
		v.m128_f32[3] = 1.0f;
		return v;
	};

	ambientColor = randomVector(rng);
	diffuseColor = randomVector(rng);
	specularColor = randomVector(rng);
	emmissionColor = randomVector(rng);
	specularExponent = rng() * 100.0f;

	return *this;
}
