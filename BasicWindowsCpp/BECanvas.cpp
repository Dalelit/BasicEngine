#include "BECanvas.h"
#include <memory.h>
#include <utility>

using namespace DirectX;

#define BE_HR_CHECK(hr) if (FAILED(hr)) throw hr; // to do: proper error checking

BECanvas::~BECanvas()
{
	if (bmpSurface) delete bmpSurface;
}

int BECanvas::Initialise(unsigned int _width, unsigned int _height)
{
	if (bmpSurface) delete bmpSurface;

	width = _width;
	height = _height;
	size = width * height;

	bmpSurface = new BESurface2D<Pixel>(width, height);

	Clear();

	return 0; // TO DO - no error checking for memory
}

void BECanvas::Clear()
{
	bmpSurface->Clear();
}
