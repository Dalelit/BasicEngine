#include "BECanvas.h"
#include <memory.h>


BECanvas::~BECanvas()
{
	if (buffer) delete buffer;
	if (bmp) delete bmp;
	if (depthBuffer) delete depthBuffer;
}

int BECanvas::Initialise(unsigned int _width, unsigned int _height)
{
	if (buffer != NULL) free(buffer);

	width = _width;
	height = _height;
	halfWidth = floorf(width * 0.5f);
	halfHeight = floorf(height * 0.5f);
	halfWH = { halfWidth, halfHeight, 1.0f };

	size = width * height;
	buffer = new Color[size];
	bmp = new Pixel[size];
	depthBuffer = NULL;

	Clear();

	return 0; // TO DO - no error checking for memory
}

void BECanvas::Clear()
{
	memset(buffer, 0, size * sizeof(Color));
	memset(bmp, 0, size * sizeof(Pixel));

	if (depthBuffer)
	{
		for (unsigned int i = 0; i < size; i++) depthBuffer[i] = defaultDepthValue;
	}
}

void BECanvas::AddDepthBuffer()
{
	depthBuffer = new float[size];
}

void BECanvas::BufferToBMP()
{
	Pixel* pp = bmp;
	Color* pc = buffer;
	Color c;

	for (unsigned int y = 0; y < size; y += width)
		for (unsigned int x = 0; x < width; x++)
		{
			c.data = pc->data * 255.0f; // to do: this line is slow
			pp->r = (unsigned char)c.r;
			pp->g = (unsigned char)c.g;
			pp->b = (unsigned char)c.b;
			pp->a = (unsigned char)c.a;

			pp++;
			pc++;
		}
}

void BECanvas::DrawLineSafe(XMVECTOR from, XMVECTOR to, XMVECTOR colorFrom, XMVECTOR colorTo)
{
	float fromX = XMVectorGetX(from);
	float fromY = XMVectorGetY(from);
	float toX = XMVectorGetX(to);
	float toY = XMVectorGetY(to);

	if (fromX < -1.0f && toX < -1.0f) return;
	if (fromY < -1.0f && toY < -1.0f) return;
	if (fromX >= 1.0f && toX >= 1.0f) return;
	if (fromY >= 1.0f && toY >= 1.0f) return;

	// convert screen space to pixels
	float x = (fromX + 1.0f) * halfWidth;
	float xt = (toX + 1.0f) * halfWidth;
	float y = (fromY + 1.0f) * halfHeight;
	float yt = (toY + 1.0f) * halfHeight;
	Color c = colorFrom;
	Color ct = colorTo;

	float dx = xt - x;
	float dy = yt - y;
	Color dc = colorTo - colorFrom;

	// special case
	// To do - optimise later
	if (fabsf(dx) < 0.5f) // change in x is less than 1 pixel... so a vertical line
	{
		if (fabsf(dy) < 0.5f) // draw a dot
		{
			buffer[(int)y * width + (int)x] = c;
			return;
		}

		dc = dc / fabsf(dy);

		if (dy > 0.0f)
		{
			
			if (y < 0.0f) y = 0.0f;  // To Do - fix color to start with jump

			while (y <= yt && y < height)
			{
				buffer[(int)y * width + (int)x] = c;
				y += 1.0f;
				c = c + dc;
			}
		}
		else if (dy < 0.0f)
		{
			if (y >= height) y = height - 1.0f;  // To Do - fix color to start with jump

			while (y >= yt && y >= 0.0f)
			{
				buffer[(int)y * width + (int)x] = c;
				y -= 1.0f;
				c = c + dc;
			}
		}
		return;
	}
	
	if (dx < 0) // swap x direction if not left to right
	{
		SWAPFLOAT(x, xt);
		dx = -dx;
		SWAPFLOAT(y, yt);
		dy = -dy;
		{Color temp = c; c = ct; ct = temp; }// to do : sort out swap!
		dc = -dc;
	}

	// special case
	// To do - optimise later
	if (fabsf(dy) < 0.5f) // change in y is less than 1 pixel, so horizontal
	{
		dc = dc / dx;

		if (x < 0.0f) x = 0.0f;  // To Do - fix color to start with jump

		while (x <= xt && x < width)
		{
			buffer[(int)y * width + (int)x] = c;
			x += 1.0f;
			c = c + dc;
		}
		return;
	}

	// set increment based on longest change
	if (dx > fabsf(dy))
	{
		dy /= dx;
		dc = dc / dx;
		dx = 1.0f;
	}
	else
	{
		dx /= fabsf(dy);
		dc = dc / fabsf(dy);
		dy = (dy > 0.0f ? 1.0f : -1.0f);
	}

	// move start point to 0.0 if off the left of the screen
	if (x < 0.0f)
	{
		// To Do - fix color to start with jump
		y += (-x) * dy; // move y along by the distance x needs to go
		x = 0.0f;
	}

	// move start point to 0.0 if off the bottom of the screen
	if (y < 0.0f)
	{
		// To Do - fix color to start with jump
		x += (-y) * dx;
		y = 0.0f;
	}

	// move start point to height if off the top of the screen
	if (y >= height)
	{
		// To Do - fix color to start with jump
		float yold = y;
		y = height - 1.0f;
		x += (yold - y) * dx;
	}

	// and finally pump out pixels
	while (x <= xt && x < width && y >= 0.0f && y < height)
	{
		buffer[(int)y * width + (int)x] = c;
		x += dx;
		y += dy;
		c = c + dc;
	}
}
