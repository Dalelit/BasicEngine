#pragma once
#include "BEModel.h"
#include "BELight.h"
#include "BESampler.h"
#include <vector>

class BEScene
{
public:
	BEScene();
	~BEScene();

	std::vector<BEModel*> models;
	std::vector<BELight*> lights;
	std::vector<BETexture*> textures;
	std::vector<BESampler*> samplers;

	std::vector<BEEntity*> entityRef; // does not own the entity... used for update

	void CreateSceneTestGround();

	void CreateSceneTestCube();
};
