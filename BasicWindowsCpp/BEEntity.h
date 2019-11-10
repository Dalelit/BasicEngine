#pragma once
#include "BEMesh.h"

class BEEntity
{
public:
	Vector3 position = Vector3::Zero;
	BEMesh* mesh = NULL;
	Color color = { 1,1,1,1 };

	inline void Translate(Vector3 v) { position += v; if (mesh != NULL) mesh->Translate(v); }
	inline void Scale(Vector3 v) { if (mesh != NULL) mesh->Scale(v); }
};

