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

void BELogger::Error(const std::string& msg)
{
	Debug(msg);
}

void BELogger::Error(const std::wstring& msg)
{
	Debug(msg);
}

void BELogger::Info(const std::string& msg)
{
	Debug(msg);
}

void BELogger::Info(const std::wstring& msg)
{
	Debug(msg);
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

	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(handle, &info)) Error("Could not get screen buffer info.");

	infoAttributes = info.wAttributes;
}

BELoggerConsole::~BELoggerConsole()
{
	FreeConsole();
}

void BELoggerConsole::Debug(const std::string& msg)
{
	SetConsoleTextAttribute(handle, debugAttributes);
	WriteConsoleA(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	NextLine();
}

void BELoggerConsole::Debug(const std::wstring& msg)
{
	SetConsoleTextAttribute(handle, debugAttributes);
	WriteConsoleW(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	NextLine();
}

void BELoggerConsole::Error(const std::string& msg)
{
	SetConsoleTextAttribute(handle, errorAttributes);
	WriteConsoleA(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	NextLine();
}

void BELoggerConsole::Error(const std::wstring& msg)
{
	SetConsoleTextAttribute(handle, errorAttributes);
	WriteConsoleW(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	NextLine();
}

void BELoggerConsole::Info(const std::string& msg)
{
	SetConsoleTextAttribute(handle, infoAttributes);
	WriteConsoleA(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	NextLine();
}

void BELoggerConsole::Info(const std::wstring& msg)
{
	SetConsoleTextAttribute(handle, infoAttributes);
	WriteConsoleW(handle, msg.c_str(), (DWORD)msg.length(), NULL, NULL);
	NextLine();
}

void BELoggerConsole::SetWindowRect(int left, int top, int width, int height)
{
	SetWindowPos(hwnd, HWND_BOTTOM, left, top, width, height, NULL);
}

inline void BELoggerConsole::NextLine()
{
	WriteConsoleA(handle, "\n", 1, NULL, NULL);
}
