#pragma once
#include <string>
#include "BEMesh.h"

class BEModelLoaderSTL
{
public:
	static BEMesh* LoadSTL(std::wstring filename);
};

