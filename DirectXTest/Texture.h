#pragma once

using namespace std;

class RenderObject;

class Texture : public RenderObject {
public:
	Texture(string name);
	void OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) override;
	void OnRender(ComPtr<ID3D11DeviceContext> d3dContext) override;
};

