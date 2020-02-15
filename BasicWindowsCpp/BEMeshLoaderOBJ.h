#pragma once
#include "pch.h"
#include "BEMesh.h"

class BEMeshLoaderOBJ
{
public:
	static std::vector<BEMesh*> LoadOBJ(std::wstring filename);
	static std::map<std::wstring, BEMaterial> LoadMTL(std::wstring filename);
};

