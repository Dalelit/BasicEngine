#pragma once

#include "pch.h"
#include "BEDirectXResource.h"
#include "BEDXTexture.h"

class BEDirectXResourceManager
{
public:
	BEDirectXDevice& device;

	BEDirectXResourceManager(BEDirectXDevice& device) : device(device) {};

	~BEDirectXResourceManager()
	{
		for (auto r : resources) delete r.second;
	}

	template <class T>
	T* AccessResource(BEDirectXDevice& device, std::string filename) {
		std::string id = T::UniqueId(filename);
		if (!ResourceExists(id))
		{
			T* ptr = new T(device, filename);
			AddResource(id, ptr);
			return ptr;
		}
		else
		{
			return (T*)GetResource(id);
		}
	}

	template <class T>
	T* AccessResource(BEDirectXDevice& device, BETexture& texture) {
		std::string id = T::UniqueId(texture.name);
		if (!ResourceExists(id))
		{
			T* ptr = new T(device, texture);
			AddResource(id, ptr);
			return ptr;
		}
		else
		{
			return (T*)GetResource(id);
		}
	}

	void AddResource(std::string id, BEDirectXResource* pResource)
	{
		assert(!ResourceExists(id));
		resources[id] = pResource;
	}

	BEDirectXResource* operator[](std::string id)
	{
		return resources[id];
	}

	BEDirectXResource* GetResource(std::string id)
	{
		return resources[id];
	}

	bool ResourceExists(std::string id)
	{
		return resources.find(id) != resources.end();
	}

private:
	std::map<std::string, BEDirectXResource*> resources;
};