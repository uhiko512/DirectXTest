#include "stdafx.h"
#include "Scene.h"


Scene::Scene(shared_ptr<Renderer>& renderer) {
	_renderer = renderer;

	auto testMesh = make_shared<MeshObject>(_renderer, "Class");

	auto deferedMesh = make_shared<MeshObject>(_renderer);
	deferedMesh->material = _renderer->GetMaterial("DeferedII");
	vector<VertexBuffer> vb;

	float z = 0.0f;
	float size = 1.0f;
	XMVECTOR vers[] = {
		XMVectorSet(-size,  size, z, 1.0f),
		XMVectorSet(size,  size, z, 1.0f),
		XMVectorSet(-size, -size, z, 1.0f),
		
		XMVectorSet(size,  size, z, 1.0f),
		XMVectorSet(size, -size, z, 1.0f),
		XMVectorSet(-size, -size, z, 1.0f)
		
	};

	XMFLOAT2 uvs[] = {
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(1.0f, 1.0f),
		XMFLOAT2(0.0f, 0.0f),
		
		XMFLOAT2(1.0f, 1.0f),
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(0.0f, 0.0f)
	};

	for (int i = 0; i < 6; i++) {
		vb.push_back({vers [i], XMVectorZero(), XMVectorZero() , uvs[i]});
	}
	deferedMesh->SetMesh(vb);
	_renderer->AddRenderObject(deferedMesh);

	auto sunLight = make_shared<Light>(_renderer);
	auto playerController = make_shared<PlayerController>();
	_renderer->SetMainCamera(playerController->GetCamera());
	AddScene(dynamic_pointer_cast<SceneObject>(playerController));
}

void Scene::Update() {
	for (auto s : _sceneObjects) {
		s->OnUpdate();
	}
}

void Scene::AddScene(shared_ptr<SceneObject>& sceneObject) { 
	_sceneObjects.push_back(sceneObject);
}
