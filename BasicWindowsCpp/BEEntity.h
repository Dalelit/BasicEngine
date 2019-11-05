#pragma once
#include "BEMesh.h"

class BEEntity
{
public:
	Vector3 position = Vector3::Zero;
	BEMesh* mesh = NULL;

	inline void Translate(Vector3 v) { position += v; if (mesh != NULL) mesh->Translate(v); }
};

