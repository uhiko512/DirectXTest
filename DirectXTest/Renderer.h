#pragma once

class Material;
class RenderObject;
class MeshObject;
class Camera;
class Light;
class SphericalHarmonicLight;

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

struct MVPConstantBuffer {
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4X4 normalMat;
};

struct CameraParams {
	XMVECTOR viewPos;
};

struct EngineParams {
	XMFLOAT4 time;
};

class Renderer {
public:
	Renderer();
	void Render();
	shared_ptr<Material> GetMaterial(string name);
	void AddRenderObject(shared_ptr<RenderObject> renderObject);
	void AddLight(shared_ptr<Light> light);
	void SetMainCamera(shared_ptr<Camera> camera);
	static void  ThrowIfFailed(HRESULT hr);

	shared_ptr<SphericalHarmonicLight> shLight;
	void SetSHLight(shared_ptr<SphericalHarmonicLight> shLight);

private:
	ComPtr<ID3D11Device>           _d3dDevice;
	ComPtr<ID3D11DeviceContext>    _d3dContext;
	ComPtr<IDXGISwapChain1>         _swapChain;
	ComPtr<ID3D11RenderTargetView> _frameBufferView;
	ComPtr<ID3D11RenderTargetView> _worldPosRTView;
	ComPtr<ID3D11RenderTargetView> _normalRTView;
	ComPtr<ID3D11RenderTargetView> _roughnessRTView;
	ComPtr<ID3D11RenderTargetView> _linearDepthRTView;
	ComPtr<ID3D11ShaderResourceView> _colorSRView;
	ComPtr<ID3D11ShaderResourceView> _worldPosSRView;
	ComPtr<ID3D11ShaderResourceView> _normalSRView;
	ComPtr<ID3D11ShaderResourceView> _roughnessSRView;
	ComPtr<ID3D11ShaderResourceView> _linearDepthSRView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	vector<shared_ptr<RenderObject>> _renderObjects;
	vector<shared_ptr<Light>> _lights;
	ComPtr<ID3D11Buffer> _mvpConstantBuffer;
	ComPtr<ID3D11Buffer> _camParamsBuffer;
	ComPtr<ID3D11Buffer> _engineParamsBuffer;
	MVPConstantBuffer _mvpConst;
	CameraParams _camParams;
	EngineParams _engineParams;
	shared_ptr<Camera> _mainCamera;
	map<string, pair<shared_ptr<Material>, vector<shared_ptr<RenderObject>>>> _materials;
	

	void InitDevice();
	void InitSwapChain();
	void InitPipeline();
	void SetupMaterial();
};



