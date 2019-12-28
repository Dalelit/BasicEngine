#pragma once
#include "BECommon.h"

// Pixel struct to make it easy to manipulate the bitmap
class BECanvas
{
public:
	struct Pixel {
		unsigned char b, g, r, a;
	};

	union Color
	{
		DirectX::XMVECTOR data;
		struct {
			float r, g, b, a;
		};

		Color() : data() {};
		Color(float _r, float _g, float _b, float _a) { data = { _r, _g , _b, _a }; };
		Color(const DirectX::XMVECTOR& v) : data (v) {}
		Color(const Color& c) : data(c.data) {};

		Color operator+(const Color& rhs) { return DirectX::XMVectorAdd(data, rhs.data); };
		Color operator-(const Color& rhs) { return DirectX::XMVectorSubtract(data,rhs.data); };
		Color operator-() { return DirectX::XMVectorNegate(data); };
		Color operator*(const float rhs) { return DirectX::XMVectorScale(data, rhs); };
		Color operator/(const float& rhs) { return DirectX::XMVectorDivide(data, DirectX::XMVectorReplicate(rhs)); };
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

	void DrawLineSafe(DirectX::XMVECTOR from, DirectX::XMVECTOR to, DirectX::XMVECTOR colorFrom, DirectX::XMVECTOR colorTo);

	inline void DrawLineSafe(DirectX::XMVECTOR from, DirectX::XMVECTOR to, DirectX::XMVECTOR color) { DrawLineSafe(from, to, color, color); };

	inline DirectX::XMVECTOR ScreenToPixel(DirectX::XMVECTOR screenPoint) { return DirectX::XMVectorMultiply(DirectX::XMVectorAdd(screenPoint, x1y1z0), halfWH); }

private:
	float halfWidth = 0;
	float halfHeight = 0;
	DirectX::XMVECTOR x1y1z0 = { 1.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR halfWH = {0,0,0,0};
};
