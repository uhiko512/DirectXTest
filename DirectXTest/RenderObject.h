#pragma once

class Material;

using namespace DirectX;
using namespace Microsoft::WRL;

class RenderObject {
public:
	virtual void OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) = 0;
	virtual void OnRender(ComPtr<ID3D11DeviceContext> d3dContext) = 0;
};
