#include "pch.h"
#include "BERenderRaytraceThread.h"


void BERenderRaytraceThread::Start()
{
	future = std::async([this]() { this->JobMain(); });
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
		future.wait();
	}
}

void BERenderRaytraceThread::JobMain()
{
	raytracer.exitLoop = false;
	running = true;

	CreateSubSections();

	std::vector<std::future<void>> futures;

	while (running && continuosLoop)
	{

		// reset things
		raytracer.ClearCanvas();
		raytracer.ResetStats();
		ResetSubSections(); 
		futures.clear();

		clock_t startTime = clock();

		auto subSectionFunc = [this](Subsection section) { this->raytracer.Draw(section.xStart, section.width, section.yStart, section.height); };

		for (auto& subsec : subsections) futures.emplace_back(std::async(subSectionFunc, subsec));

		for (auto& f : futures) f.wait();

		float durationSeconds = (float)(clock() - startTime) / (float)CLOCKS_PER_SEC;

		// copy the result to the target and call the function for when it's finished
		if (pResultCanvas) pResultCanvas->Copy(raytracer.GetCanvas());
		if (callback) callback(L"Render time: " + std::to_wstring(durationSeconds) + L"s");
	}

	running = false;
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
