#pragma once
#include "pch.h"
#include "BEMesh.h"

class BEMeshLoaderPLY
{
public:
	static BEMesh* LoadPLY(std::wstring filename);
};

