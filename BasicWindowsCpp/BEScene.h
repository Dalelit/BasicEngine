#pragma once
#include "BEModel.h"
#include "BELight.h"
#include "BESampler.h"

class BEScene
{
public:
	BEScene();
	~BEScene();

	BELightAmbient ambientLight;

	BELightDirectional directionalLight;

	std::vector<BEModel*> models;
	std::vector<BEPointLight*> lights;
	std::vector<BETexture*> textures;
	std::vector<BESampler*> samplers;

	void Update(float deltaTime);

	void Clear();

	void ShowImguiEnvironment();
	void ShowImguiEntities();
};
