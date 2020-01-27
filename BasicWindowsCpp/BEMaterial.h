#pragma once
#include "BESampler.h"
#include <string>

class BEMaterial
{
public:
	DirectX::XMVECTOR ambientColor = { 1,1,1,1 };
	DirectX::XMVECTOR diffuseColor = { 1,1,1,1 };
	DirectX::XMVECTOR specularColor = { 1,1,1,1 };
	DirectX::XMVECTOR emmissionColor = { 0,0,0,1 };
	float specularExponent = 10.0f;
	//float transparency = 1.0f; // maybe do this one day
	BESampler* pTextureSampler = nullptr;
	std::wstring name = L"Default";

	BEMaterial() = default;
	BEMaterial(std::wstring name) : name(name) {};

	inline bool IsTextured() { return pTextureSampler != nullptr; }
};

