#pragma once
#include "BEMesh.h"

class BEEntity
{
public:
	DirectX::XMVECTOR position = { 0,0,0,0 };
	BEMesh* mesh = nullptr;

	inline void SetColor(DirectX::XMVECTOR color) { if (mesh) mesh->SetColor(color); }

	inline void Translate(DirectX::XMVECTOR v) { position = DirectX::XMVectorAdd(position, v); if (mesh) mesh->Translate(v); }
	inline void Scale(float f) { if (mesh) mesh->Scale(f); }
	inline void Scale(DirectX::XMVECTOR v) { if (mesh) mesh->Scale(v); }

	inline void Transform(DirectX::XMMATRIX m) { position = XMVector3Transform(position, m); if (mesh) mesh->Transform(m); };
};

