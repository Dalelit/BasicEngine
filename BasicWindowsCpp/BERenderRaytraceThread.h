#pragma once
#include "BERenderRaytrace.h"
#include <thread>
#include <mutex>
#include <vector>

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

	unsigned int numberOfThreads = 4;
	unsigned int subsectionsWide = 8;
	unsigned int subsectionsHigh = 6;

	void Start();

	void Stop();
	void StopAndWait();

	void SetContinousLoop() { continuosLoop = true; }
	void StopContinousLoop() { continuosLoop = false; }

	bool IsRunning() { return running; }

	void(*callback)(std::wstring msg);

private:
	BERenderRaytrace& raytracer;
	bool running = false;
	bool continuosLoop = true;

	void JobMain();
	void JobSubsection();
	void CreateSubSections();
	void ResetSubSections();
	Subsection* TakeSubSection();

	std::thread thread;
	std::mutex mutex;
	std::vector<Subsection> subsections;
	std::vector<std::thread> threads;

	int threadCount = 0;
};

