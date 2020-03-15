#pragma once

#include "BEDirectXResource.h"

class BEDirectXShader : public BEDirectXResource
{
public:
	bool LogShader();

protected:
	BEDirectXShader() = default;

	wrl::ComPtr<ID3DBlob> pBlob = nullptr;

};
