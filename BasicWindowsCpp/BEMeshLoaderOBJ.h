#pragma once
#include <string>
#include <map>
#include "BEMesh.h"

class BEMeshLoaderOBJ
{
public:
	static BEMesh* LoadOBJ(std::wstring filename);
	static std::map<std::wstring, BEMaterial> LoadMTL(std::wstring filename);
};

