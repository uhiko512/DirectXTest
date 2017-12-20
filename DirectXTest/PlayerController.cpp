#include "stdafx.h"
#include "PlayerController.h"


PlayerController::PlayerController() { 
	_camera = make_shared<Camera>();
	isMouseLock = false;
	Mouse::State mouseState = Mouse::Get().GetState();
	_prevMousePos = XMVectorSet(mouseState.x, mouseState.y, 0.0f, 0.0f);
}

shared_ptr<Camera> PlayerController::GetCamera() {
	return _camera;
}

void PlayerController::OnUpdate() {

	Keyboard::State keyboardState = Keyboard::Get().GetState();
	XMVECTOR offsetT = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	if (keyboardState.A) {
		offsetT += XMVectorSet(-0.1f, 0.0f, 0.0f, 0.0f);
	} else if (keyboardState.D) {
		offsetT += XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
	}
	if (keyboardState.W) {
		offsetT += XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f);
	} else if (keyboardState.S) {
		offsetT += XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);
	}
	Translate(XMVector3Transform(offsetT, XMMatrixRotationRollPitchYawFromVector(transform.rotation * (XM_PI / 180.0f))));

	if (keyboardState.Escape) {
		isMouseLock = !isMouseLock;
	}

	if (!isMouseLock) {
		Mouse::State mouseState = Mouse::Get().GetState();
		XMVECTOR currentMousePos = XMVectorSet(mouseState.x, mouseState.y, 0.0f, 0.0f);
		if (XMVector2NotEqual(currentMousePos, _prevMousePos)) {
			XMFLOAT2 offsetR;
			XMStoreFloat2(&offsetR, currentMousePos - _prevMousePos);
			Rotate(XMVectorSet(offsetR.y * -0.5f * ((abs(_camera->transform.rotation.m128_f32[0] + offsetR.y * -0.5f) >= 85.0f)?0.0f:1.0f), offsetR.x * -0.5f, 0.0f, 0.0f));
		}
		_prevMousePos = currentMousePos;
	}

	_camera->SetTransform(transform);
}
