#pragma once
#include "SceneObject.h"

class Camera;

class PlayerController : public SceneObject {
public:
	bool isMouseLock;
	PlayerController();
	shared_ptr<Camera> GetCamera();
	void OnUpdate() override;

private:
	shared_ptr<Camera> _camera;
	XMVECTOR _prevMousePos;
};

