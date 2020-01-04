#pragma once

#include <string>
#include "BESurface2D.h"
#include <DirectXMath.h>

class BETexture
{
public:
	BETexture(std::wstring filename);
	~BETexture() {
		if (surface) delete surface;
	}

	unsigned int GetWidth() { return width; }
	unsigned int GetHeight() { return height; }

	BESurface2D<DirectX::XMVECTOR>* surface = nullptr; // to do: better storage solution

protected:
	unsigned int width;
	unsigned int height;
};
