#pragma once
#include "pch.h"
#include "BEMesh.h"

class BEMeshLoaderSTL
{
public:
	static BEMesh* LoadSTL(std::wstring filename);
};

