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

	void SetContinousLoop() { continuosLoop = true; }
	void StopContinousLoop() { continuosLoop = false; }

	bool IsRunning() { return running; }

	void(*callback)(std::wstring msg);
	BECanvas* pResultCanvas = nullptr;

private:
	BERenderRaytrace& raytracer;
	bool running = false;
	bool continuosLoop = true;
	std::future<void> future;
	std::vector<Subsection> subsections;

	void JobMain();
	void CreateSubSections();
	void ResetSubSections();

};

