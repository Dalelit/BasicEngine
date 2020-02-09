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

	BEMaterial();
	BEMaterial(std::wstring name);

	inline bool IsTextured() { return pTextureSampler != nullptr; }

	void Randomise();

protected:
	static inline long instanceCounter = 0;
};

