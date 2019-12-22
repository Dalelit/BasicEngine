#pragma once

#include <string>
#include "BECanvas.h"

class BETexture
{
public:
	BETexture(std::wstring filename);
	~BETexture() = default;

	unsigned int height;
	unsigned int width;

	BECanvas canvas; // to do: better storage solution
};
