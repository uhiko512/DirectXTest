#pragma once

class AbsShaderCreator;

class VertexShaderCreator : public AbsShaderCreator {
public:
	VertexShaderCreator();

protected:
	HRESULT Handle(ComPtr<ID3D11Device> d3dDevice, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) override;
	void Handle(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11DeviceChild* shader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override;

};

