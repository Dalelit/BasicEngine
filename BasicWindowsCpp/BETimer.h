#pragma once

#include "pch.h"

class BETimer
{
public:
	clock_t currentTime;
	clock_t previousTime;

	BETimer() { Start(); }

	inline int ElapsedMilSec() { return currentTime - previousTime; }
	inline float ElapsedSec() { return (float)(currentTime - previousTime) / (float)CLOCKS_PER_SEC; }

	inline BETimer& Tick() { currentTime = clock(); return *this; }

	inline void Start() { previousTime = currentTime = clock(); }

	// in seconds
	inline float DeltaTime() {
		previousTime = currentTime;
		currentTime = clock();
		return (float)(currentTime - previousTime) / 1000.0f;
	}
};