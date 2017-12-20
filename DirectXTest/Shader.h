#pragma once

/*enum ShaderType {
	VertexShader,
	HullShader,
	DomainShader,
	GeometryShader,
	PixelSahder,
	ComputerShader
};*/
// VS, HS, DS, GS, PS, CS

using namespace std;
using namespace DirectX;

class AbsShaderCreator;
class VertexShaderCreator;
class PixelShaderCreator;

class Shader : public RenderObject {
public:
	Shader(string name, vector<string> shaderTypes = {"VS", "PS"});
	void OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) override;
	void OnRender(ComPtr<ID3D11DeviceContext> d3dContext) override;
	void CreateShader(ComPtr<ID3D11Device> d3dDevice, string type, const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage);
	void SetPiplineShader(ComPtr<ID3D11DeviceContext> d3dContext, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
	static vector<byte> ReadShaderByteCode(string filename);

public:
	string Name;
	vector<string> ShaderTypes;
	vector<ComPtr<ID3D11DeviceChild>> Shaders;

private:
	vector<shared_ptr<AbsShaderCreator>> ShaderCreators;
};
