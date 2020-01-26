#pragma once
#include <string>
#include "BEMesh.h"

class BEMeshLoaderPLY
{
public:
	static BEMesh* LoadPLY(std::wstring filename);
};

