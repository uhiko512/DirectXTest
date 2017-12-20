#pragma once

class SceneObject;
class Renderer;

using namespace DirectX;

struct LookAtParams {
	XMVECTOR eye;
	XMVECTOR focus;
	XMVECTOR up;
};

class Camera : public SceneObject {
public:
	Camera();
	XMMATRIX GetViewMatrix();

public:
	LookAtParams lookAtParams;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX orthographicMatrix;
};

