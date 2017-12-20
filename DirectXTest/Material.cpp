#include "stdafx.h"
#include "Material.h"

Material::Material(ComPtr<ID3D11Device> d3dDevice, string matName) {
	_name = matName;
	
	tex = make_shared<Texture>("WashedGravel");

	shad = make_shared<Shader>(matName);
	shad->OnInitRenderer(d3dDevice);

	/**/
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	vector<byte> vertexShaderCode = Shader::ReadShaderByteCode("InputLayoutVS.cso");
	Renderer::ThrowIfFailed(
		d3dDevice->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderCode.data(), vertexShaderCode.size(), &_inputLayout)
	);
	/**/

	CD3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dDevice->CreateSamplerState(&samplerDesc, &_samplerState);
}

string Material::GetName() {
	return _name;
}

void Material::SetupParams(ComPtr<ID3D11DeviceContext> d3dContext, shared_ptr<ObjectParams> &params) {
	////_d3dContext->VSSetConstantBuffers(1, 1, _lightConstantBuffer.GetAddressOf());

	d3dContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
	d3dContext->PSSetShaderResources(0, 1, params->baseColor.GetAddressOf());
	d3dContext->PSSetSamplers(1, 1, _samplerState.GetAddressOf());
	d3dContext->PSSetShaderResources(1, 1, params->normal.GetAddressOf());
	d3dContext->PSSetSamplers(2, 1, _samplerState.GetAddressOf());
	d3dContext->PSSetShaderResources(2, 1, params->height.GetAddressOf());
	d3dContext->PSSetSamplers(3, 1, _samplerState.GetAddressOf());
	d3dContext->PSSetShaderResources(3, 1, params->roughness.GetAddressOf());
}

void Material::SetupMaterial(ComPtr<ID3D11DeviceContext> d3dContext) {
	/**/
	d3dContext->IASetInputLayout(_inputLayout.Get());
	/**/
	shad->OnRender(d3dContext);
}

