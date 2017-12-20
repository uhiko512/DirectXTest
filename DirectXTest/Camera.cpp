#include "stdafx.h"
#include "Camera.h"


Camera::Camera() {
	/*if (isMainCamera) {
		renderer->SetMainCamera(shared_ptr<Camera>(this));
	}*/
	projectionMatrix = XMMatrixPerspectiveFovRH(55.0f * (XM_PI / 180.0f), ((float) Win32App::Width) / Win32App::Height, 0.01f, 1000.0f);
	orthographicMatrix = XMMatrixOrthographicRH(Win32App::Width, Win32App::Height, 0.01f, 1000.0f);
}

XMMATRIX Camera::GetViewMatrix() {
	lookAtParams.eye = transform.position;
	lookAtParams.focus = lookAtParams.eye + XMVector3Transform(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), XMMatrixRotationRollPitchYawFromVector(transform.rotation * (XM_PI / 180.0f)));
	lookAtParams.up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	return XMMatrixLookAtRH(lookAtParams.eye, lookAtParams.focus, lookAtParams.up);
}
