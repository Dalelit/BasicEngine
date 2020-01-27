#pragma once
#include <string>
#include "BEMesh.h"

class BEMeshLoaderOBJ
{
public:
	static BEMesh* LoadOBJ(std::wstring filename);
};

