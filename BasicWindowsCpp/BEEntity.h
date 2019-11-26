#pragma once
#include "BEMesh.h"

class BEEntity
{
public:
	XMVECTOR position = { 0,0,0,0 };
	BEMesh* mesh = NULL;
	XMVECTOR color = { 1,1,1,1 };

	inline void Translate(XMVECTOR v) { position += v; if (mesh) mesh->Translate(v); }
	inline void Scale(float f) { if (mesh) mesh->Scale(f); }
	inline void Scale(XMVECTOR v) { if (mesh) mesh->Scale(v); }

	inline void Transform(XMMATRIX m) { position = XMVector3Transform(position, m); if (mesh) mesh->Transform(m); };
};

