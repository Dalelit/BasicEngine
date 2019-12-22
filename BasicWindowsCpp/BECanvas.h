#pragma once
#include "BECommon.h"

using namespace DirectX;

// Pixel struct to make it easy to manipulate the bitmap
class BECanvas
{
public:
	struct Pixel {
		unsigned char b, g, r, a;
	};

	union Color
	{
		XMVECTOR data;
		struct {
			float r, g, b, a;
		};

		Color() : data() {};
		Color(float _r, float _g, float _b, float _a) { data = { _r, _g , _b, _a }; };
		Color(const XMVECTOR& v) : data (v) {}
		Color(const Color& c) : data(c.data) {};

		Color operator+(const Color& rhs) { return Color(data + rhs.data); };
		Color operator-(const Color& rhs) { return Color(data - rhs.data); };
		Color operator-() { return Color(-data); };
		Color operator*(const float rhs) { return Color(data * rhs); };
		Color operator/(const float& rhs) { return Color(data / rhs); };
	};

	Color* buffer = NULL;
	Pixel* bmp = NULL;
	float* depthBuffer = NULL;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;

	unsigned int GetBitmapPitch() const { return width * sizeof(Pixel); };
	unsigned int GetBufferPitch() const { return width * sizeof(Color); };

	unsigned int GetBitmapSize() const { return width * height * sizeof(Pixel); };
	unsigned int GetBufferSize() const { return width * height * sizeof(Color); };

	float defaultDepthValue = FLT_MAX;

	~BECanvas();

	int Initialise(unsigned int width, unsigned int height);

	void AddDepthBuffer();

	void Clear();

	void BufferToBMP();

	void DrawLineSafe(XMVECTOR from, XMVECTOR to, XMVECTOR colorFrom, XMVECTOR colorTo);

	inline void DrawLineSafe(XMVECTOR from, XMVECTOR to, XMVECTOR color) { DrawLineSafe(from, to, color, color); };

	inline XMVECTOR ScreenToPixel(XMVECTOR screenPoint) { return (screenPoint + x1y1z0) * halfWH; }

private:
	float halfWidth = 0;
	float halfHeight = 0;
	XMVECTOR x1y1z0 = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMVECTOR halfWH = {0,0,0,0};
};
