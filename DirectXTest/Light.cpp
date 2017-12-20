#include "stdafx.h"
#include "Light.h"


Light::Light(shared_ptr<Renderer> renderer) {
	_camera = make_shared<Camera>();
	lightParams = make_shared<LightParams>();
	lightParams->position = _camera->lookAtParams.eye;
	lightParams->direction = _camera->lookAtParams.focus;
	lightParams->lightMatrix = _camera->viewMatrix * _camera->projectionMatrix;

	renderer->AddLight(shared_ptr<Light>(this));
}
