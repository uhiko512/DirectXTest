#include "stdafx.h"
#include "SceneObject.h"


SceneObject::SceneObject() { }

XMMATRIX SceneObject::GetModelMatrix() {
	XMMATRIX modelMat = XMMatrixScalingFromVector(transform.scale) * XMMatrixRotationRollPitchYawFromVector(transform.rotation * (XM_PI / 180.0f)) * XMMatrixTranslationFromVector(transform.position);
	
	if (parentNode == nullptr) {
		return modelMat;
	}

	return  modelMat * parentNode->GetModelMatrix();
}

void SceneObject::SetTransform(Transform &transform) {
	this->transform = transform;
}

void SceneObject::Translate(XMVECTOR offset) { 
	transform.position += offset;
}

void SceneObject::Rotate(XMVECTOR offset) {
	transform.rotation += offset;
}
