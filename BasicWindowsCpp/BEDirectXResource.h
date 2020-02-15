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
	static std::wstring GenerateUniqueId(std::wstring str) {
		std::string tname(typeid(T).name());
		std::wstring id(tname.begin() + 6, tname.end()); // convert to wstring and remove "class ". To do: A better way?
		id += L"#";
		id += str;
		return id;
	};

};