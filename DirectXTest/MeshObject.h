#pragma once

namespace fbxsdk{
	class FbxNode;
}

struct VertexBuffer;
struct ObjectParams;
class RenderObject;
class SceneObject;
class Camera;
class Model;


using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

class MeshObject : public SceneObject, public RenderObject, public enable_shared_from_this<MeshObject> {
public:
	MeshObject(shared_ptr<Renderer> renderer);
	MeshObject(shared_ptr<Renderer> renderer, string filename);
	static XMVECTOR FbxVector2XMVector(FbxVector4 vector);
	static XMMATRIX FbxMatrix2XMMatrix(FbxAMatrix matrix);
	void OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) override;
	void OnRender(ComPtr<ID3D11DeviceContext> d3dContext) override;
	void SetMesh(vector<VertexBuffer> mesh);
	void SetModel(shared_ptr<Model> model);
	
	FbxNode *LoadFbx(const char *filename);

public:
	shared_ptr<Material> material;

private:
	void TraceNode(FbxNode *currentNode, shared_ptr<MeshObject> parant);

private:
	shared_ptr<Model> _model;
	ComPtr<ID3D11Buffer> _vertexBuffer;
	vector<VertexBuffer> _mesh;
	shared_ptr<Renderer> _renderer;
	shared_ptr<ObjectParams> _matParams;

};

