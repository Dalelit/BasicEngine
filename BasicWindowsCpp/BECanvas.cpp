#include "BECanvas.h"


int BECanvas::Initialise(unsigned int _width, unsigned int _height)
{
	if (buffer != NULL) free(buffer);

	width = _width;
	height = _height;
	halfWidth = floorf(width * 0.5f);
	halfHeight = floorf(height * 0.5f);
	halfWH = { halfWidth, halfHeight, 1.0f };

	// to do: work out what mem allocation to use... malloc? new? etc?
	size = width * height;
	buffer = (Color*)malloc(size * sizeof(Color));
	bmp = (Pixel*)malloc(size * sizeof(Pixel));
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
		// to do : work out an efficient method
		memset(depthBuffer, defaultDepthValue, size);
		for (unsigned int i = 0; i < size; i++) depthBuffer[i] = defaultDepthValue;
	}
}

void BECanvas::AddDepthBuffer()
{
	depthBuffer = (float*)malloc(size * sizeof(float));
}

// To Do - optimise or is there another way? Especially Color -> Pixel conversion
void BECanvas::BufferToBMP()
{
	Pixel* pp = bmp;
	Color* pc = buffer;

	for (unsigned int y = 0; y < size; y += width)
		for (unsigned int x = 0; x < width; x++)
		{
			XMFLOAT4 xmf = *pc;
			pp->r = (unsigned char)(xmf.x * 255);
			pp->g = (unsigned char)(xmf.y * 255);
			pp->b = (unsigned char)(xmf.z * 255);
			pp->a = (unsigned char)(xmf.w * 255);

			pp++;
			pc++;
		}
}

void BECanvas::DrawLineSafe(Vector3 from, Vector3 to, Color colorFrom, Color colorTo)
{
	if (from.x < -1.0f && to.x < -1.0f) return;
	if (from.y < -1.0f && to.y < -1.0f) return;
	if (from.x >= 1.0f && to.x >= 1.0f) return;
	if (from.y >= 1.0f && to.y >= 1.0f) return;

	// convert screen space to pixels
	float x = (from.x + 1.0f) * halfWidth;
	float xt = (to.x + 1.0f) * halfWidth;
	float y = (from.y + 1.0f) * halfHeight;
	float yt = (to.y + 1.0f) * halfHeight;
	Color c = colorFrom;
	Color ct = colorTo;

	float dx = xt - x;
	float dy = yt - y;
	Color dc = colorTo - colorFrom;

	// special case
	// To do - optimise later
	if (abs(dx) < 0.5f) // change in x is less than 1 pixel... so a vertical line
	{
		if (abs(dy) < 0.5f) // draw a dot
		{
			buffer[(int)y * width + (int)x] = c;
			return;
		}

		dc = dc / abs(dy);

		if (dy > 0.0f)
		{
			
			if (y < 0.0f) y = 0.0f;  // To Do - fix color to start with jump

			while (y <= yt && y < height)
			{
				buffer[(int)y * width + (int)x] = c;
				y += 1.0f;
				c += dc;
			}
		}
		else if (dy < 0.0f)
		{
			if (y >= height) y = height - 1.0f;  // To Do - fix color to start with jump

			while (y >= yt && y >= 0.0f)
			{
				buffer[(int)y * width + (int)x] = c;
				y -= 1.0f;
				c += dc;
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
		SWAPCOLOR(c, ct);
		dc = -dc;
	}

	// special case
	// To do - optimise later
	if (abs(dy) < 0.5f) // change in y is less than 1 pixel, so horizontal
	{
		dc = dc / dx;

		if (x < 0.0f) x = 0.0f;  // To Do - fix color to start with jump

		while (x <= xt && x < width)
		{
			buffer[(int)y * width + (int)x] = c;
			x += 1.0f;
			c += dc;
		}
		return;
	}

	// set increment based on longest change
	if (dx > abs(dy))
	{
		dy /= dx;
		dc = dc / dx;
		dx = 1.0f;
	}
	else
	{
		dx /= abs(dy);
		dc = dc / abs(dy);
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
		c += dc;
	}
}

void BECanvas::Tests()
{
	Color c = {1,1,1};
	Vector3 zero = { 0,0,0 };
	Vector3 limit = { (float)width, (float)height ,0 };


	// Testing DrawLineSafe
	DrawLineSafe(zero, limit, c);
	DrawLineSafe({0, (float)height, 0}, limit / 2, c);
	DrawLineSafe({ 10, 0,0 }, { 10, 100, 0 }, { 0,1,0 });
	DrawLineSafe({ 20, 100,0 }, { 20, 0, 0 }, { 0,1,0 });
	DrawLineSafe({ 30, -100,0 }, { 30, height + 100.0f, 0 }, { 0,1,0 });
	DrawLineSafe({ 40, height + 100.0f, 0 }, { 40, -100.0f, 0 }, { 0,1,0 });
	DrawLineSafe({ 0, 120,0 }, { 100, 120, 0 }, { 0,1,0 });
	DrawLineSafe({ 100, 140,0 }, { 0, 140, 0 }, { 0,1,0 });
	DrawLineSafe({ -50, 160,0 }, { width + 50.0f, 160, 0 }, { 0,1,0 });
	DrawLineSafe({ width + 50.0f, 180,0 }, { -50, 180, 0 }, { 0,1,0 });

	DrawLineSafe({ 320, 20,0 }, { 330, 400, 0 }, { 0,0,1 });
	DrawLineSafe({ 320, 400,0 }, { 330, 20, 0 }, { 0,0,1 });

	DrawLineSafe({ width / 2.0f - 50.0f, -50, 0 }, limit / 3, { 1,0,0 });
	DrawLineSafe({ width / 2.0f + 50.0f, -50, 0 }, limit / 3, { 1,0,0 });
	DrawLineSafe({ width / 2.0f - 50.0f, height + 50.0f, 0 }, limit / 3, { 1,0,0 });
	DrawLineSafe({ width / 2.0f + 50.0f, height + 50.0f, 0 }, limit / 3, { 1,0,0 });

	DrawLineSafe({ -50.0f, height / 2.0f + 50, 0 }, limit / 4, { 1,1,0 });
	DrawLineSafe({ -50.0f, height / 2.0f - 50, 0 }, limit / 4, { 1,1,0 });
	DrawLineSafe({ width + 50.0f, height / 2.0f + 50, 0 }, 0.75f * limit, { 1,1,0 });
	DrawLineSafe({ width + 50.0f, height / 2.0f - 50, 0 }, 0.75f * limit, { 1,1,0 });
}

