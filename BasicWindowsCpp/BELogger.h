#pragma once

#include "pch.h"

// Useage:
// 1. Pick type of logger
//    BELogger - base type, writes to vs debug output
//    BELoggerConsole - creates a console and write to it. Console dissappears when finished.
// 2. Call BELoggerXXXX::Init();
// 3. Use macros when you want to log a message

#define BELOG_DEBUG(MSG) BELogger::GetInstance()->Debug(MSG)

// Base class logger and outputs to VisStu's bebug output.
class BELogger
{
public:
	static BELogger* GetInstance();

	static void Init();

	BELogger();
	virtual ~BELogger();

	virtual void Debug(const std::string& msg);
	virtual void Debug(const std::wstring& msg);

protected:
	static std::unique_ptr<BELogger> instance;

};


// Console version. Creates a console to output to.
class BELoggerConsole : public BELogger
{
public:
	static void Init();

	BELoggerConsole();
	~BELoggerConsole();

	virtual void Debug(const std::string& msg);
	virtual void Debug(const std::wstring& msg);

	void SetWindowRect(int left, int top, int width, int height);

protected:
	HANDLE handle = NULL;
	HWND hwnd = NULL;
};
