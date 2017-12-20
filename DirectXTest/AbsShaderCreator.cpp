#include "stdafx.h"
#include "AbsShaderCreator.h"

void AbsShaderCreator::SetNext(shared_ptr<AbsShaderCreator> next) { 
	NextShader = next;
}

HRESULT AbsShaderCreator::CreateShader(ComPtr<ID3D11Device> d3dDevice, string type, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) {
	if (ShaderType != type) {
		return ToNext(d3dDevice, type, pShaderBytecode, BytecodeLength, pClassLinkage);
	}

	IsCreated = true;

	return Handle(d3dDevice, pShaderBytecode, BytecodeLength, pClassLinkage);
}

void AbsShaderCreator::SetPiplineShader(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) {
	Handle(d3dContext, IsCreated ? Shader.Get() : nullptr, ppClassInstances, NumClassInstances);
	ToNext(d3dContext, ppClassInstances, NumClassInstances);
}

HRESULT AbsShaderCreator::ToNext(ComPtr<ID3D11Device> d3dDevice, string type, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) {
	if (NextShader.get()) {
		return NextShader->CreateShader(d3dDevice, type, pShaderBytecode, BytecodeLength, pClassLinkage);
	}
	
	return E_INVALIDARG;
}

void AbsShaderCreator::ToNext(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) {
	if (NextShader.get()) {
		NextShader->SetPiplineShader(d3dContext, ppClassInstances, NumClassInstances);
	}
}
