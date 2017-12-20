#pragma once

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

class Shader;
class Texture;

struct ObjectParams {
	ComPtr<ID3D11ShaderResourceView> baseColor;
	ComPtr<ID3D11ShaderResourceView> normal;
	ComPtr<ID3D11ShaderResourceView> height;
	ComPtr<ID3D11ShaderResourceView> roughness;
};

class Material {
public:
	Material(ComPtr<ID3D11Device> d3dDevice, string matName);
	shared_ptr<Shader> shad;
	shared_ptr<Texture> tex;
	string GetName();
	void SetupParams(ComPtr<ID3D11DeviceContext> d3dContext, shared_ptr<ObjectParams> &params);
	void SetupMaterial(ComPtr<ID3D11DeviceContext> d3dContext);

private:
	string _name;
	ComPtr<ID3D11InputLayout>  _inputLayout;
	ComPtr<ID3D11SamplerState> _samplerState;
	ComPtr<ID3D11Buffer> _mvpConstantBuffer;
};

