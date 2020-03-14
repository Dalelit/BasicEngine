#pragma once

#include "pch.h"
#include "BEDirectXDevice.h"

class BEDirectXResource
{
public:
	virtual void Bind(BEDirectXDevice& device) = 0;
	virtual ~BEDirectXResource() = default;

protected:

	template <class T>
	static std::string GenerateUniqueId(std::string str) {
		std::string tname(typeid(T).name());
		std::string id(tname.begin() + 6, tname.end()); // convert to wstring and remove "class ". To do: A better way?
		id += "#";
		id += str;
		return id;
	};

};