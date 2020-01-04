#pragma once
#include <assert.h>

template <class T>
class BESurface2D
{
public:

	BESurface2D(unsigned int width, unsigned int height) :
		width(width), height(height),
		elementSize(sizeof(T)),
		size(width*height),
		pitchBytes(width * elementSize),
		totalBytes(size * elementSize)
	{
		assert(width > 0 && height > 0);
		data = new T[size];
		assert(data != nullptr);
	}

	~BESurface2D() {
		delete data;
	}

	inline unsigned int GetWidth() { return width; }
	inline unsigned int GetHeight() { return height; }

	inline T* GetData() const { return data; }
	inline T* GetData(unsigned int x, unsigned int y) { return &(data[y * width + x]); }

	inline unsigned int GetSize() const { return size; }
	inline unsigned int GetPitchBytes() const { return pitchBytes; }
	inline unsigned int GetTotalBytes() const { return totalBytes; }

	inline void Clear() {
		memset(data, 0, totalBytes);
	}

	inline void Clear(T value) {
		for (unsigned int i = 0; i < size; i++)data[i] = value;
	}

	inline void SetValue(unsigned int x, unsigned int y, T value) {
		data[y * width + x] = value;
	}

	inline T GetValue(unsigned int x, unsigned int y) {
		return data[y * width + x];
	}

protected:
	T* data = nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int elementSize = 0;
	unsigned int size = 0;
	unsigned int pitchBytes = 0;
	unsigned int totalBytes = 0;
};