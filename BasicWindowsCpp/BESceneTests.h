#pragma once

#include "BEScene.h"
#include "BECamera.h"

class BESceneTests
{
public:
	static void CreateSceneTest0(BEScene& scene);
	static void CreateSceneTest1(BEScene& scene);
	static void CreateSceneTest2(BEScene& scene);
	static void CreateSceneTest3(BEScene& scene);

	static void CreateBoxWorld(BEScene& scene, BECamera& camera);
};

