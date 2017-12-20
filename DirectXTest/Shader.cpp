#include "stdafx.h"
#include "Shader.h"


Shader::Shader(string name, vector<string> shaderTypes) {
	Name = name;
	ShaderTypes = shaderTypes;

	ShaderCreators.push_back(dynamic_pointer_cast<AbsShaderCreator>(make_shared<VertexShaderCreator>()));
	ShaderCreators.push_back(dynamic_pointer_cast<AbsShaderCreator>(make_shared<PixelShaderCreator>()));
	for (auto i = 1; i < ShaderCreators.size(); i++) {
		ShaderCreators[i - 1]->SetNext(dynamic_pointer_cast<AbsShaderCreator>(ShaderCreators[i]));
	}
}

void Shader::OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) {
	Shaders.resize(ShaderTypes.size());
	for (auto type : ShaderTypes) {
		vector<byte> shaderCode = ReadShaderByteCode(Name + type + ".cso");
		CreateShader(d3dDevice, type, shaderCode.data(), shaderCode.size(), nullptr);
	}
}

void Shader::OnRender(ComPtr<ID3D11DeviceContext> d3dContext) {
	SetPiplineShader(d3dContext, nullptr, 0);
}

void Shader::CreateShader(ComPtr<ID3D11Device> d3dDevice, string type, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage) {
	Renderer::ThrowIfFailed(
		ShaderCreators[0]->CreateShader(d3dDevice, type, pShaderBytecode, BytecodeLength, pClassLinkage)
	);
}

void Shader::SetPiplineShader(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) {
	ShaderCreators[0]->SetPiplineShader(d3dContext, ppClassInstances, NumClassInstances);
}

vector<byte> Shader::ReadShaderByteCode(string filename) {
	vector<byte> shaderByteCode;

	ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		int length = (int)file.tellg();

		shaderByteCode = vector<byte>(length);
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char *>(shaderByteCode.data()), length);
		file.close();
	}

	return shaderByteCode;
}
