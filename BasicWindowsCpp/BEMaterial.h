#pragma once
#include "pch.h"
#include "BESampler.h"
#include "BEUtil.h"

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
	std::string name;

	BEMaterial(std::string name);
	BEMaterial(std::wstring name) : BEMaterial(BEUtil::ToString(name)) {};
	BEMaterial();
	// to do: copy constructor should technically increment instance counter.

	inline bool IsTextured() { return pTextureSampler != nullptr; }

	BEMaterial& Randomise(bool normalizeColor = false);

	static BEMaterial CreateRandom(bool normalizeColor = false) { return BEMaterial().Randomise(normalizeColor); };

	void ShowImguiTreeNode();

protected:
	static unsigned int materialCount;
};

