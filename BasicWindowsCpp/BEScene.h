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

	DirectX::XMVECTOR ambientLight = { 0.2f, 0.2f, 0.2f, 1.0f };

	BELightDirectional directionalLight;

	std::vector<BEModel*> models;
	std::vector<BEPointLight*> lights;
	std::vector<BETexture*> textures;
	std::vector<BESampler*> samplers;

	std::vector<BEEntity*> entityRef; // does not own the entity... used for update

	void Update(float deltaTime);

	void CreateSceneTest0();
	void CreateSceneTest1();
	void CreateSceneTest2();
	void CreateSceneTest3();
};
