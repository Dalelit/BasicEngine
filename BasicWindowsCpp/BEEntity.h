#pragma once
#include "BEMesh.h"

class BEEntity
{
public:
	Vector3 position = Vector3::Zero;
	BEMesh* mesh = NULL;
	Color color = { 1,1,1,1 };

	inline void Translate(Vector3 v) { position += v; if (mesh) mesh->Translate(v); }
	inline void Scale(float f) { if (mesh) mesh->Scale(f); }
	inline void Scale(Vector3 v) { if (mesh) mesh->Scale(v); }

	inline void Transform(Matrix m) { position = Vector3::Transform(position, m); if (mesh) mesh->Transform(m); };
};

