#pragma once

namespace fbxsdk {
	class FbxNode;
}

struct VertexBuffer {
	XMVECTOR position;
	XMVECTOR normal;
	XMVECTOR tangent;
	XMFLOAT2 uv;
};

class Model : public RenderObject {
public:
	Model();
	void OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) override;
	void OnRender(ComPtr<ID3D11DeviceContext> d3dContext) override;
	void LoadMesh(FbxMesh* fbxMesh);

private:
	static bool _isInputLayoutCreated;
	static bool _isInputLayoutSetted;
	ComPtr<ID3D11InputLayout>  _inputLayout;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	vector<VertexBuffer> _mesh;
	
};

