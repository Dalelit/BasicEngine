#include "pch.h"
#include "BEDirectXShader.h"

bool BEDirectXShader::LogShader()
{
	BELOG_INFO("--- Logging shader -------------------------");

	std::stringstream msg;

	HRESULT hr;

	wrl::ComPtr<ID3D11ShaderReflection> pReflection = nullptr;
	hr = D3DReflect(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		&pReflection
	);

	if (FAILED(hr)) { BELOG_INFO("Failed to get shader reflection"); return false; }

	D3D11_SHADER_DESC desc;
	pReflection->GetDesc(&desc);
	msg << "Shader description" << std::endl;
	msg << " ConstantBuffers: " << desc.ConstantBuffers << std::endl;
	msg << " InputParameters: " << desc.InputParameters << std::endl;
	msg << " OutputParameters: " << desc.OutputParameters << std::endl;
	//msg << " Version " << desc.Version << std::endl;

	for (UINT i = 0; i < desc.ConstantBuffers; i++)
	{
		msg << "Constant buffer: " << i << std::endl;

		D3D11_SHADER_BUFFER_DESC cbDesc;
		ID3D11ShaderReflectionConstantBuffer* pConstBuffer = pReflection->GetConstantBufferByIndex(i);
		pConstBuffer->GetDesc(&cbDesc);
		msg << " Name: " << cbDesc.Name << std::endl;
		//msg << " Type" << cbDesc.Type << std::endl;
		msg << " Variables: " << cbDesc.Variables << std::endl;

		for (UINT j = 0; j < cbDesc.Variables; j++)
		{
			ID3D11ShaderReflectionVariable* pVariable = pConstBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC varDesc;
			pVariable->GetDesc(&varDesc);
			msg << "  Name: " << varDesc.Name;
			msg << "  Size: " << varDesc.Size;
			msg << "  Offset: " << varDesc.StartOffset << std::endl;
		}
	}

	msg << "Input parameters" << std::endl;
	for (UINT i = 0; i < desc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetInputParameterDesc(i, &paramDesc);
		msg << " " << i;
		msg << " SemanticName: " << paramDesc.SemanticName;
		msg << " SemanticIndex: " << paramDesc.SemanticIndex;
		msg << " Register: " << paramDesc.Register;
		msg << " SystemValueType: " << paramDesc.SystemValueType;
		msg << " ComponentType: " << paramDesc.ComponentType << std::endl;
	}

	msg << "Output parameters" << std::endl;
	for (UINT i = 0; i < desc.OutputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflection->GetOutputParameterDesc(i, &paramDesc);
		msg << " " << i;
		msg << " SemanticName: " << paramDesc.SemanticName;
		msg << " SemanticIndex: " << paramDesc.SemanticIndex;
		msg << " Register: " << paramDesc.Register;
		msg << " SystemValueType: " << paramDesc.SystemValueType;
		msg << " ComponentType: " << paramDesc.ComponentType << std::endl;
	}

	UINT slotCount = pReflection->GetNumInterfaceSlots();
	msg << "Slot count " << slotCount << std::endl;

	BELOG_INFO(msg.str());

	//pReflection->GetInputParameterDesc(0, &inputDesc);

	BELOG_INFO("--------------------------------------------");

	return false;
}
