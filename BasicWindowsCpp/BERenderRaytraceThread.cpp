#include "BERenderRaytraceThread.h"
#include <time.h>


void BERenderRaytraceThread::Start()
{
	raytracer.exitLoop = false;
	running = true;

	if (thread.joinable()) thread.detach(); // to do: does this leak?
	thread = std::thread([this]() { this->JobMain(); });
}

void BERenderRaytraceThread::Stop()
{
	running = false;
	raytracer.exitLoop = true;
}

void BERenderRaytraceThread::StopAndWait()
{
	if (running)
	{
		Stop();
		if (thread.joinable()) thread.join();
	}
}

void BERenderRaytraceThread::JobMain()
{
	threadCount++;

	CreateSubSections();

	while (running && continuosLoop)
	{
		clock_t startTime = clock();

		// reset things
		raytracer.ClearCanvas();
		raytracer.ResetStats();
		ResetSubSections(); 
		threads.clear(); // just making sure nothing is lingering around if stop/start happens quickly

		// kick off the threads
		for (unsigned int i = 0; i < numberOfThreads; i++)
		{
			threads.emplace_back([this]() { this->JobSubsection(); });
		}

		// wait till they finish
		for (auto& t : threads) if (t.joinable()) t.join();
		threads.clear();

		float durationSeconds = (float)(clock() - startTime) / (float)CLOCKS_PER_SEC;

		// call the function for when it's finished
		if (callback) callback(L"Render time: " + std::to_wstring(durationSeconds) + L"s");
	}

	running = false;

	threadCount--;
}

void BERenderRaytraceThread::JobSubsection()
{
	threadCount++;

	Subsection* s = TakeSubSection();

	while (running && s)
	{
		raytracer.Draw(s->xStart, s->width, s->yStart, s->height);
		s = TakeSubSection();
	}

	threadCount--;
}

void BERenderRaytraceThread::CreateSubSections()
{
	subsections.clear();

	unsigned int sectionWidth = raytracer.GetWidth() / subsectionsWide;
	unsigned int sectionHeight = raytracer.GetHeight() / subsectionsHigh;

	Subsection arg;
	arg.available = true;
	arg.height = sectionHeight;
	arg.width = sectionWidth;

	for (unsigned int y = 0; y < subsectionsHigh; y++)
	{
		arg.yStart = y * sectionHeight;
		for (unsigned int x = 0; x < subsectionsWide; x++)
		{
			arg.xStart = x * sectionWidth;
			subsections.emplace_back(arg);
		}
	}
	std::shuffle(subsections.begin(), subsections.end(), std::random_device());
}

void BERenderRaytraceThread::ResetSubSections()
{
	for (auto& s : subsections) s.available = true; // make all available
}

BERenderRaytraceThread::Subsection* BERenderRaytraceThread::TakeSubSection()
{
	Subsection* section = nullptr;

	std::lock_guard lock(mutex);

	auto s = subsections.begin();

	while (s != subsections.end() && !s->available) s++; // find an available one

	if (s != subsections.end()) // found one
	{
		section = s._Ptr;
		section->available = false; // claim it
	}

	return section;
}
