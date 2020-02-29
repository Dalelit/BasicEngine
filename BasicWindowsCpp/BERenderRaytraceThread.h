#pragma once
#include "pch.h"
#include "BERenderRaytrace.h"

class BERenderRaytraceThread
{
public:
	struct Subsection
	{
		unsigned int xStart;
		unsigned int width;
		unsigned int yStart;
		unsigned int height;
		bool available;
	};

	BERenderRaytraceThread(BERenderRaytrace& raytracer) : raytracer(raytracer) {}

	unsigned int subsectionsWide = 8;
	unsigned int subsectionsHigh = 6;

	void Start();

	void Stop();
	void StopAndWait();

	bool IsRunning() { return running; }

	BERenderRaytrace& GetRaytracer() { return raytracer; }

	std::wstring GetStats();

private:
	BERenderRaytrace& raytracer;

	bool running = false;
	float durationSeconds = 0;

	std::future<void> future;
	std::vector<Subsection> subsections;

	void JobMain();
	void CreateSubSections();
	void ResetSubSections();

};

