#include "pch.h"
#include "BERenderRaytraceThread.h"


void BERenderRaytraceThread::Start()
{
	running = true;
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

std::wstring BERenderRaytraceThread::GetStats()
{
	std::wstring msg;

	if (running)
	{
		msg = raytracer.GetWorkingStats();
	}
	else
	{
		msg = L"Render time: " + std::to_wstring(durationSeconds) + L"s";
	}

	return msg;
}

void BERenderRaytraceThread::JobMain()
{
	raytracer.exitLoop = false;

	CreateSubSections();

	std::vector<std::future<void>> futures;

	// reset things
	raytracer.ClearCanvas();
	raytracer.ResetStats();

	clock_t startTime = clock();

	auto subSectionFunc = [this](Subsection section) { this->raytracer.Draw(section.xStart, section.width, section.yStart, section.height); };

	for (auto& subsec : subsections) futures.emplace_back(std::async(subSectionFunc, subsec));

	for (auto& f : futures) f.wait();

	durationSeconds = (float)(clock() - startTime) / (float)CLOCKS_PER_SEC;

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
