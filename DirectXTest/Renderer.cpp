#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer() {
	_mainCamera = make_shared<Camera>();
	InitDevice();
	InitSwapChain();
	InitPipeline();
	SetupMaterial();
}

void Renderer::Render() {
	float ClearColor[4] = {0.27f, 0.7f, 1.0f, 1.0f};
	
	float ClearNormal[4] = {0.0f, 0.0f, 0.5f, 0.0f};
	float ClearRoughness = 0.0f;
	float ClearLinearDepth = 1.0f;

	RECT rect = Win32App::GetScreenRect();
	CD3D11_VIEWPORT viewport = CD3D11_VIEWPORT(rect.left, rect.top, Win32App::Width, Win32App::Height);

	// init
	_d3dContext->RSSetViewports(1, &viewport);
	/*
	_d3dContext->OMSetRenderTargets(2, _frameBufferView.GetAddressOf(), _depthStencilView.Get());
	_d3dContext->ClearRenderTargetView(_frameBufferView.Get(), ClearColor);
	_d3dContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0);
	/**/
	/**/
	ID3D11RenderTargetView* rtvs[] = {
		_frameBufferView.Get(),
		_normalRTView.Get(),
		_roughnessRTView.Get(),
		_worldPosRTView.Get(),
		_linearDepthRTView.Get()
	};
	_d3dContext->OMSetRenderTargets(5, rtvs, _depthStencilView.Get());
	_d3dContext->ClearRenderTargetView(_frameBufferView.Get(), ClearColor);
	_d3dContext->ClearRenderTargetView(_normalRTView.Get(), ClearNormal);
	_d3dContext->ClearRenderTargetView(_roughnessRTView.Get(), &ClearRoughness);
	_d3dContext->ClearRenderTargetView(_linearDepthRTView.Get(), &ClearLinearDepth);
	_d3dContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0);
	/**/

	// setting
	_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_d3dContext->VSSetConstantBuffers(0, 1, _mvpConstantBuffer.GetAddressOf());
	_d3dContext->VSSetConstantBuffers(1, 1, _camParamsBuffer.GetAddressOf());
	_d3dContext->PSSetConstantBuffers(0, 1, _camParamsBuffer.GetAddressOf());
	_d3dContext->PSSetConstantBuffers(1, 1, _engineParamsBuffer.GetAddressOf());

	// render scene
	XMMATRIX viewMat = _mainCamera->GetViewMatrix();
	XMStoreFloat4x4(&_mvpConst.view, viewMat);
	XMStoreFloat4x4(&_mvpConst.projection, _mainCamera->projectionMatrix);
	_d3dContext->UpdateSubresource(_mvpConstantBuffer.Get(), 0, nullptr, &_mvpConst, 0, 0);

	_camParams.viewPos = _mainCamera->transform.position;
	_d3dContext->UpdateSubresource(_camParamsBuffer.Get(), 0, nullptr, &_camParams, 0, 0);
	//_lights[0]->lightMatrix;

	//for (auto mat : _materials) {
	auto mat = _materials["DeferedI"];
	mat.first->SetupMaterial(_d3dContext);
	for (auto renderObj : mat.second) {
		XMMATRIX modelMat = dynamic_pointer_cast<MeshObject>(renderObj)->GetModelMatrix();
		XMStoreFloat4x4(&_mvpConst.model, modelMat);

		XMMATRIX mvMat = modelMat;
		XMVECTOR det = XMMatrixDeterminant(mvMat);
		XMMATRIX normalMat = XMMatrixInverse(&det, mvMat);
		XMStoreFloat4x4(&_mvpConst.normalMat, normalMat);
			
		_d3dContext->UpdateSubresource(_mvpConstantBuffer.Get(), 0, nullptr, &_mvpConst, 0, 0);
		renderObj->OnRender(_d3dContext);
	}

	/**/
	ComPtr<ID3D11Resource> backBufferTex2D;
	_frameBufferView->GetResource(&backBufferTex2D);

	ComPtr<ID3D11Resource> colorBufferTex2D;
	_colorSRView->GetResource(&colorBufferTex2D);

	_d3dContext->CopyResource(colorBufferTex2D.Get(), backBufferTex2D.Get());


	_d3dContext->OMSetRenderTargets(1, _frameBufferView.GetAddressOf(), _depthStencilView.Get());

	mat = _materials["DeferedII"];
	mat.first->SetupMaterial(_d3dContext);
	_d3dContext->PSSetShaderResources(0, 1, _colorSRView.GetAddressOf());
	_d3dContext->PSSetShaderResources(1, 1, _normalSRView.GetAddressOf());
	_d3dContext->PSSetShaderResources(2, 1, _worldPosSRView.GetAddressOf());
	_d3dContext->PSSetShaderResources(3, 1, _roughnessSRView.GetAddressOf());
	_d3dContext->PSSetShaderResources(4, 1, _linearDepthSRView.GetAddressOf());
	for (auto renderObj : mat.second) {
		renderObj->OnRender(_d3dContext);
	}
	_d3dContext->PSSetShaderResources(4, 0, nullptr);
	/**/
	//}

	_swapChain->Present(0, 0);

	_engineParams.time.x += 0.1f;
	_d3dContext->UpdateSubresource(_engineParamsBuffer.Get(), 0, nullptr, &_engineParams, 0, 0);
}

shared_ptr<Material> Renderer::GetMaterial(string name) {
	return _materials.at(name).first;
}

void Renderer::AddRenderObject(shared_ptr<RenderObject> renderObject) {
	shared_ptr<MeshObject> meshObject = dynamic_pointer_cast<MeshObject>(renderObject);

	if (!meshObject->material) {
		meshObject->material = GetMaterial("DeferedI");
	}
	renderObject->OnInitRenderer(_d3dDevice);
	_materials.at(meshObject->material->GetName()).second.push_back(renderObject);

}

void Renderer::AddLight(shared_ptr<Light> light) {
	_lights.push_back(light);
}

void Renderer::SetMainCamera(shared_ptr<Camera> camera) { 
	_mainCamera = camera;
}

void Renderer::InitDevice() { 
	ThrowIfFailed(
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&_d3dDevice,
			nullptr,
			&_d3dContext
		)
	);
}

void Renderer::InitSwapChain() {
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
	swapChainDesc.Width = Win32App::Width;
	swapChainDesc.Height = Win32App::Height;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	ComPtr<IDXGIDevice3> dxgiDevice;
	ComPtr<IDXGIAdapter> adapter;
	ComPtr<IDXGIFactory2> factory;
	ThrowIfFailed(
		_d3dDevice.As(&dxgiDevice)
	);
	ThrowIfFailed(
		dxgiDevice->GetAdapter(&adapter)
	);
	ThrowIfFailed(
		adapter->GetParent(IID_PPV_ARGS(&factory))
	);
	ThrowIfFailed(
		factory->CreateSwapChainForHwnd(
			_d3dDevice.Get(),
			Win32App::GethWnd(),
			&swapChainDesc,
			nullptr,
			nullptr,
			_swapChain.GetAddressOf()
		)
	);
}

void Renderer::InitPipeline() {
	ComPtr<ID3D11RasterizerState> rasterizerState;
	CD3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	rasterizerDesc.FrontCounterClockwise = false;
	ThrowIfFailed(
		_d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState)
	);
	_d3dContext->RSSetState(rasterizerState.Get());

	// Swap Chain Back Buffer
	ComPtr<ID3D11Texture2D> backBufferTex2D;
	ThrowIfFailed(
		_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBufferTex2D)
	);
	ThrowIfFailed(
		_d3dDevice->CreateRenderTargetView(backBufferTex2D.Get(), nullptr, &_frameBufferView)
	);


	ComPtr<ID3D11Texture2D> colorBufferTex2D;
	ComPtr<ID3D11Resource> frameBufferTex2D;
	_frameBufferView->GetResource(&frameBufferTex2D);
	D3D11_TEXTURE2D_DESC colorBufferTex2DDesc;
	((ID3D11Texture2D*)frameBufferTex2D.Get())->GetDesc(&colorBufferTex2DDesc);
	colorBufferTex2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ThrowIfFailed(
		_d3dDevice->CreateTexture2D(&colorBufferTex2DDesc, nullptr, &colorBufferTex2D)
	);

	ThrowIfFailed(
		_d3dDevice->CreateShaderResourceView(colorBufferTex2D.Get(), nullptr, &_colorSRView)
	);

	ComPtr<ID3D11Texture2D> worldPosBufferTex2D;
	CD3D11_TEXTURE2D_DESC worldPosBufferTex2DDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R16G16B16A16_FLOAT, Win32App::Width, Win32App::Height);
	worldPosBufferTex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ThrowIfFailed(
		_d3dDevice->CreateTexture2D(&worldPosBufferTex2DDesc, nullptr, &worldPosBufferTex2D)
	);
	ThrowIfFailed(
		_d3dDevice->CreateRenderTargetView(worldPosBufferTex2D.Get(), nullptr, &_worldPosRTView)
	);

	ThrowIfFailed(
		_d3dDevice->CreateShaderResourceView(worldPosBufferTex2D.Get(), nullptr, &_worldPosSRView)
	);

	ComPtr<ID3D11Texture2D> normalBufferTex2D;
	CD3D11_TEXTURE2D_DESC normalBufferTex2DDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R16G16B16A16_FLOAT, Win32App::Width, Win32App::Height);
	normalBufferTex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ThrowIfFailed(
		_d3dDevice->CreateTexture2D(&normalBufferTex2DDesc, nullptr, &normalBufferTex2D)
	);
	ThrowIfFailed(
		_d3dDevice->CreateRenderTargetView(normalBufferTex2D.Get(), nullptr, &_normalRTView)
	);

	ThrowIfFailed(
		_d3dDevice->CreateShaderResourceView(normalBufferTex2D.Get(), nullptr, &_normalSRView)
	);

	ComPtr<ID3D11Texture2D> roughnessBufferTex2D;
	CD3D11_TEXTURE2D_DESC roughnessBufferTex2DDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R16_FLOAT, Win32App::Width, Win32App::Height);
	roughnessBufferTex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ThrowIfFailed(
		_d3dDevice->CreateTexture2D(&roughnessBufferTex2DDesc, nullptr, &roughnessBufferTex2D)
	);
	ThrowIfFailed(
		_d3dDevice->CreateRenderTargetView(roughnessBufferTex2D.Get(), nullptr, &_roughnessRTView)
	);

	ThrowIfFailed(
		_d3dDevice->CreateShaderResourceView(roughnessBufferTex2D.Get(), nullptr, &_roughnessSRView)
	);

	ComPtr<ID3D11Texture2D> linearDepthBufferTex2D;
	CD3D11_TEXTURE2D_DESC linearDepthBufferTex2DDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R16_FLOAT, Win32App::Width, Win32App::Height);
	linearDepthBufferTex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ThrowIfFailed(
		_d3dDevice->CreateTexture2D(&linearDepthBufferTex2DDesc, nullptr, &linearDepthBufferTex2D)
	);
	ThrowIfFailed(
		_d3dDevice->CreateRenderTargetView(linearDepthBufferTex2D.Get(), nullptr, &_linearDepthRTView)
	);

	ThrowIfFailed(
		_d3dDevice->CreateShaderResourceView(linearDepthBufferTex2D.Get(), nullptr, &_linearDepthSRView)
	);

	// Depth Stencil Buffer
	ComPtr<ID3D11Texture2D> depthStencilTex2D;
	CD3D11_TEXTURE2D_DESC depthStencilTex2DDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D32_FLOAT_S8X24_UINT, Win32App::Width, Win32App::Height);
	depthStencilTex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ThrowIfFailed(
		_d3dDevice->CreateTexture2D(&depthStencilTex2DDesc, nullptr, &depthStencilTex2D)
	);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(depthStencilTex2D.Get(), D3D11_DSV_DIMENSION_TEXTURE2D);
	ThrowIfFailed(
		_d3dDevice->CreateDepthStencilView(depthStencilTex2D.Get(), &depthStencilViewDesc, &_depthStencilView)
	);

	CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
	ComPtr<ID3D11DepthStencilState> depthStencilState;
	ThrowIfFailed(
		_d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState)
	);

	_d3dContext->OMSetDepthStencilState(depthStencilState.Get(), 1);


	XMStoreFloat4x4(&_mvpConst.model, XMMatrixIdentity());
	XMStoreFloat4x4(&_mvpConst.view, _mainCamera->viewMatrix);
	XMStoreFloat4x4(&_mvpConst.projection, _mainCamera->projectionMatrix);
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA mvpData = {0};
	mvpData.pSysMem = &_mvpConst;
	Renderer::ThrowIfFailed(
		_d3dDevice->CreateBuffer(&constantBufferDesc, &mvpData, &_mvpConstantBuffer)
	);

	_camParams.viewPos = _mainCamera->transform.position;
	CD3D11_BUFFER_DESC camParamsBufferDesc(sizeof(CameraParams), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA camData = {0};
	camData.pSysMem = &_camParams;
	Renderer::ThrowIfFailed(
		_d3dDevice->CreateBuffer(&camParamsBufferDesc, &camData, &_camParamsBuffer)
	);

	_engineParams.time.x = 0.0f;
	CD3D11_BUFFER_DESC engineParamsBufferDesc(sizeof(EngineParams), D3D11_BIND_CONSTANT_BUFFER);
	D3D11_SUBRESOURCE_DATA engineData = {0};
	engineData.pSysMem = &_engineParams;
	Renderer::ThrowIfFailed(
		_d3dDevice->CreateBuffer(&engineParamsBufferDesc, &engineData, &_engineParamsBuffer)
	);
}

void Renderer::SetupMaterial() {
	string matNames[] = {
		//"Forward",
		"DeferedI",
		"DeferedII"
	};

	for (auto name : matNames) {
		auto mat = make_shared<Material>(_d3dDevice, name);
		_materials.insert({name, {mat, vector<shared_ptr<RenderObject>>()}});
	}
}

void Renderer::ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		LPVOID pText = 0;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPWSTR) &pText, 0, nullptr);
		OutputDebugString(L"ThrowIfFailed: ");
		OutputDebugString((LPCWSTR) pText);
		throw exception();
	}
}

void Renderer::SetSHLight(shared_ptr<SphericalHarmonicLight> shLight) { 
	this->shLight = shLight;
	//shLight->SetupEnvMap(_d3dDevice, _d3dContext);
}
