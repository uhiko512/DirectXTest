#include "stdafx.h"
#include "PixelShaderCreator.h"


PixelShaderCreator::PixelShaderCreator() { 
	ShaderType = "PS";
}

HRESULT PixelShaderCreator::Handle(ComPtr<ID3D11Device> d3dDevice, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) {
	return d3dDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, (ID3D11PixelShader**) ((ID3D11DeviceChild**) &Shader));
}

void PixelShaderCreator::Handle(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11DeviceChild* shader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) {
	d3dContext->PSSetShader((ID3D11PixelShader*) shader, ppClassInstances, NumClassInstances);
}


