#include "stdafx.h"
#include "VertexShaderCreator.h"


VertexShaderCreator::VertexShaderCreator() { 
	ShaderType = "VS";
}

HRESULT VertexShaderCreator::Handle(ComPtr<ID3D11Device> d3dDevice, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) {
	return d3dDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, (ID3D11VertexShader**) ((ID3D11DeviceChild**) &Shader));
}

void VertexShaderCreator::Handle(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11DeviceChild* shader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) {
	d3dContext->VSSetShader((ID3D11VertexShader*) shader, ppClassInstances, NumClassInstances);
}
