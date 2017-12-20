#include "stdafx.h"
#include "SphericalHarmonicLight.h"
//#include 

SphericalHarmonicLight::SphericalHarmonicLight() { 
	
}

void SphericalHarmonicLight::SHProjectEnvMap(ComPtr<ID3D11DeviceContext> d3dContext) {
	/*Renderer::ThrowIfFailed(
		SHProjectCubeMap(d3dContext.Get(), 3, envMap.Get(), resultR.data(), resultG.data(), resultB.data())
	);*/
}

void SphericalHarmonicLight::SetupEnvMap(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext) {
	Image cubeImage[6];
	ScratchImage cube;
	cube.InitializeCube(DXGI_FORMAT_R16G16B16A16_FLOAT, 32, 32, 1, 1);
	TexMetadata texMeta = cube.GetMetadata();
	
	//texMeta.dimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	wstring filename[6] = {
		L"Texture/WoodBaseColor.tga",
		L"Texture/WoodBaseColor.tga",
		L"Texture/WoodBaseColor.tga",
		L"Texture/WoodBaseColor.tga",
		L"Texture/WoodBaseColor.tga",
		L"Texture/WoodBaseColor.tga"
	};

	ScratchImage image[6];
	for (auto i = 0; i < 6; i++) {
		Renderer::ThrowIfFailed(
			LoadFromTGAFile(filename[i].c_str(), nullptr, image[i])
		);
		cubeImage[i] = image[i].GetImages()[0];
	}

	Renderer::ThrowIfFailed(
		CreateShaderResourceView(d3dDevice.Get(), cubeImage, 6, texMeta, &EnvMapView)
	);

	/*<ID3D11Resource> envMapRes;
	EnvMapView->GetResource(&envMapRes);

	Renderer::ThrowIfFailed(
		SHProjectCubeMap(d3dContext.Get(), 3, (ID3D11Texture2D *) envMapRes.Get(), resultR.data(), resultG.data(), resultB.data())
	);*/
}

