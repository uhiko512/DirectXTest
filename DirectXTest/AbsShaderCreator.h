#pragma once

using namespace std;

class AbsShaderCreator {
public:
	void SetNext(shared_ptr<AbsShaderCreator> next);

	HRESULT CreateShader(ComPtr<ID3D11Device> d3dDevice, string type, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage);
	void SetPiplineShader(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);

protected:
	virtual HRESULT Handle(ComPtr<ID3D11Device> d3dDevice, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) = 0;
	HRESULT ToNext(ComPtr<ID3D11Device> d3dDevice, string type, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage);
	virtual void Handle(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11DeviceChild* shader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) = 0;
	void ToNext(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);

protected:
	shared_ptr<AbsShaderCreator> NextShader;

	string ShaderType;
	bool IsCreated;
	ComPtr<ID3D11DeviceChild> Shader;
	
};