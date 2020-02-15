#include "pch.h"
#include "BEMaterial.h"

using namespace DirectX;

BEMaterial::BEMaterial(std::wstring name)
{
	id = L"BEMaterial" + name + std::to_wstring(++instanceCounter);
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
