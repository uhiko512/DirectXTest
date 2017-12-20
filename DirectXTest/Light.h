#pragma once

class SceneObject;
class Renderer;

using namespace std;

struct LightParams {
	float intensity = 1.0f;
	XMVECTOR position;
	XMVECTOR direction;
	XMMATRIX lightMatrix;
};

class Light : public SceneObject {
public:
	Light(shared_ptr<Renderer> renderer);
	shared_ptr<LightParams> lightParams;

private:
	shared_ptr<Camera> _camera;
};

