#pragma once

using namespace DirectX;

struct Transform {
	XMVECTOR position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR scale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
};

class SceneObject {
public:
	string name;
	Transform transform;
	shared_ptr<SceneObject> parentNode;
	vector<shared_ptr<SceneObject>> childNodes;
	SceneObject();
	XMMATRIX GetModelMatrix();
	void SetTransform(Transform &transform);
	void Translate(XMVECTOR offset);
	void Rotate(XMVECTOR offset);
	virtual void OnUpdate() { };
};

