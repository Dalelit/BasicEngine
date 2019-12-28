#pragma once
#include <string>
#include "BEMesh.h"

class BEMeshLoaderSTL
{
public:
	static BEMesh* LoadSTL(std::wstring filename);
};

