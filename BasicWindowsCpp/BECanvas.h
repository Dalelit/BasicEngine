#pragma once
#include "BECommon.h"


using namespace DirectX;

// Pixel struct to make it easy to manipulate the bitmap
class BECanvas
{
public:
	union Pixel
	{
		int data;
		struct {
			unsigned char b, g, r, a;
		};

		Pixel() { data = 0; }
		Pixel(unsigned int _data) { data = _data; }
		Pixel(unsigned char _r, unsigned char _g, unsigned char _b) { r = _r; b = _b; g = _g; a = 255; }
	};

	union Color
	{
		XMFLOAT4 data;
		struct {
			float r, g, b, a;
		};

		Color() { data = { 0,0,0,0 }; };
		Color(float _r, float _g, float _b, float _a) { data = { _r, _g , _b, _a }; };
		Color(XMVECTOR v) { XMStoreFloat4(&data, v); }

		Color operator+(const Color& rhs) { return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); };
		Color operator*(const float& rhs) { return Color(r * rhs, g * rhs, b * rhs, a * rhs); };
		Color operator+=(const Color& rhs) { r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a; };
		Color operator*=(const float& rhs) { r *= rhs; g *= rhs; b *= rhs; a *= rhs; return *this; };
		Color operator-(const Color& rhs) { return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a); };
		Color operator-() { return Color(-r, -g, -b, -a); };
		Color operator/(const float& rhs) { return Color(r / rhs, g / rhs, b / rhs, a / rhs); };
	};

	Color* buffer = NULL;
	Pixel* bmp = NULL;
	float* depthBuffer = NULL;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;

	float defaultDepthValue = FLT_MAX;

	int Initialise(unsigned int width, unsigned int height);

	void AddDepthBuffer();

	void Clear();

	void BufferToBMP();

	void DrawLineSafe(XMVECTOR from, XMVECTOR to, XMVECTOR colorFrom, XMVECTOR colorTo);

	inline void DrawLineSafe(XMVECTOR from, XMVECTOR to, XMVECTOR color) { DrawLineSafe(from, to, color, color); };

	inline XMVECTOR ScreenToPixel(XMVECTOR screenPoint) { return (screenPoint + x1y1z0) * halfWH; }

	void Tests();

private:
	float halfWidth;
	float halfHeight;
	XMVECTOR x1y1z0 = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMVECTOR halfWH;
};
