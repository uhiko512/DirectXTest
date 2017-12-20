#pragma once
#include "SceneObject.h"

class SphericalHarmonicLight : public SceneObject {
public:
	SphericalHarmonicLight();
	vector<float> resultR;
	vector<float> resultG;
	vector<float> resultB;
	ComPtr<ID3D11Texture2D> envMap;
	ComPtr<ID3D11ShaderResourceView> EnvMapView;
	void SetupEnvMap(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext);
	void SHProjectEnvMap(ComPtr<ID3D11DeviceContext> d3dContext);
};

