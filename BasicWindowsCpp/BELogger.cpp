#include "pch.h"
#include "BELogger.h"
#include <debugapi.h>

std::unique_ptr<BELogger> BELogger::instance;

void BELogger::Init()
{
	instance = std::make_unique<BELogger>();
}

BELogger* BELogger::GetInstance()
{
	return instance.get();
}

void BELogger::Debug(const std::string& msg)
{
	OutputDebugStringA(msg.c_str());
	OutputDebugStringA("\n");
}

void BELogger::Debug(const std::wstring& msg)
{
	OutputDebugStringW(msg.c_str());
	OutputDebugStringW(L"\n");
}

BELogger::BELogger()
{
}

BELogger::~BELogger()
{
}

/////////////////////////////////////////////////////

void BELoggerConsole::Init()
{
	instance = std::make_unique<BELoggerConsole>();
}

BELoggerConsole::BELoggerConsole()
{
	if (!AllocConsole()) throw "Could not create console";

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	hwnd = GetConsoleWindow();

	if (handle == INVALID_HANDLE_VALUE || handle == NULL || hwnd == NULL) throw "Could not get console handle";
}

BELoggerConsole::~BELoggerConsole()
{
	FreeConsole();
}

void BELoggerConsole::Debug(const std::string& msg)
{
	WriteConsoleA(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	WriteConsoleA(handle, "\n", 1, NULL, NULL);
}

void BELoggerConsole::Debug(const std::wstring& msg)
{
	WriteConsoleW(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	WriteConsoleW(handle, L"\n", 1, NULL, NULL);
}

void BELoggerConsole::SetWindowRect(int left, int top, int width, int height)
{
	SetWindowPos(hwnd, HWND_BOTTOM, left, top, width, height, NULL);
}
