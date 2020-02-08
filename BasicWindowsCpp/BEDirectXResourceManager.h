#pragma once

#include "BEDirectXResource.h"
#include "BEDXTexture.h"
#include <string>
#include <memory>
#include <map>
#include <cassert>

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
	T* AccessResource(BEDirectXDevice& device, std::wstring filename) {
		std::wstring id = T::UniqueId(filename);
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
		std::wstring id = T::UniqueId(texture.name);
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

	void AddResource(std::wstring id, BEDirectXResource* pResource)
	{
		assert(!ResourceExists(id));
		resources[id] = pResource;
	}

	BEDirectXResource* operator[](std::wstring id)
	{
		return resources[id];
	}

	BEDirectXResource* GetResource(std::wstring id)
	{
		return resources[id];
	}

	bool ResourceExists(std::wstring id)
	{
		return resources.find(id) != resources.end();
	}

private:
	std::map<std::wstring, BEDirectXResource*> resources;
};