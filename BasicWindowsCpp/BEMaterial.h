#pragma once
#include "BESampler.h"
#include <string>

class BEMaterial
{
public:
	DirectX::XMVECTOR ambientColor = { 1,1,1,1 };
	DirectX::XMVECTOR diffuseColor = { 1,1,1,1 };
	DirectX::XMVECTOR specularColor = { 1,1,1,1 };
	DirectX::XMVECTOR emmissionColor = { 0,0,0,1 }; // to do: not used yet
	float specularExponent = 40.0f;
	//float transparency = 1.0f; // maybe do this one day
	BESampler* pTextureSampler = nullptr;
	std::wstring id;

	BEMaterial(std::wstring name);
	BEMaterial() : BEMaterial(L"") {};
	// to do: copy constructor should technically increment instance counter.

	inline bool IsTextured() { return pTextureSampler != nullptr; }

	BEMaterial& Randomise(bool normalizeColor = false);

	static BEMaterial CreateRandom(bool normalizeColor = false) { return BEMaterial().Randomise(normalizeColor); };

protected:
	static inline long instanceCounter = 0;
};

