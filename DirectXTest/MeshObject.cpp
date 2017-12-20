#include "stdafx.h"
#include "MeshObject.h"


MeshObject::MeshObject(shared_ptr<Renderer> renderer) {
	_renderer = renderer;
	_matParams = make_shared<ObjectParams>();
}

MeshObject::MeshObject(shared_ptr<Renderer> renderer, string filename) : MeshObject(renderer) {
	_model = make_shared<Model>();
	LoadFbx(("Mesh/" + filename + ".fbx").data());
}

XMVECTOR MeshObject::FbxVector2XMVector(FbxVector4 vector) {
	return XMVectorSet(
			vector.mData[0],
			vector.mData[1],
			vector.mData[2],
			1.0f
		);
}

XMMATRIX MeshObject::FbxMatrix2XMMatrix(FbxAMatrix matrix) {
	XMMATRIX xmmat;
	for (int i = 0; i < 4; i++) {
		xmmat.r[i] = XMVectorSet(
				matrix.Get(0, i),
				matrix.Get(1, i),
				matrix.Get(2, i),
				matrix.Get(3, i)
			);
	}

	return xmmat;
}

void MeshObject::OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) {
	//_model->OnInitRenderer(d3dDevice);
	/**/
	D3D11_BUFFER_DESC bufferDesc = {0};
	D3D11_SUBRESOURCE_DATA verticesData = {0};
	bufferDesc.ByteWidth = sizeof(VertexBuffer) * _mesh.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	verticesData.pSysMem = _mesh.data();
	Renderer::ThrowIfFailed(
		d3dDevice->CreateBuffer(&bufferDesc, &verticesData, &_vertexBuffer)
	);
	/**/

	ScratchImage image;
	Renderer::ThrowIfFailed(
		LoadFromTGAFile(L"Texture/WoodBaseColor.tga", nullptr, image)
	);
	Renderer::ThrowIfFailed(
		CreateShaderResourceView(d3dDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &_matParams->baseColor)
	);

	Renderer::ThrowIfFailed(
		LoadFromTGAFile(L"Texture/n.tga", nullptr, image)
	);
	Renderer::ThrowIfFailed(
		CreateShaderResourceView(d3dDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &_matParams->normal)
	);

	Renderer::ThrowIfFailed(
		LoadFromTGAFile(L"Texture/h.tga", nullptr, image)
	);
	Renderer::ThrowIfFailed(
		CreateShaderResourceView(d3dDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &_matParams->height)
	);

	Renderer::ThrowIfFailed(
		LoadFromTGAFile(L"Texture/WoodRoughness.tga", nullptr, image)
	);
	Renderer::ThrowIfFailed(
		CreateShaderResourceView(d3dDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &_matParams->roughness)
	);
	//OutputDebugStringA((to_string(image.GetMetadata().format) + ", " + to_string(image.GetMetadata().height)).data());
	
}

void MeshObject::OnRender(ComPtr<ID3D11DeviceContext> d3dContext) {
	if (material) {
		if (material->GetName() == "DeferedI") {
			material->SetupParams(d3dContext, _matParams);
		}
		//_model->OnRender(d3dContext);
		/**/
		UINT stride = sizeof(VertexBuffer);
		UINT offset = 0;
		d3dContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		d3dContext->Draw(_mesh.size(), 0);
		/**/
		/*OutputDebugStringA(to_string(_mesh.size()).c_str());
		OutputDebugString(L"\n");*/
	}
}

/**/
void MeshObject::SetMesh(vector<VertexBuffer> mesh) {
	_mesh = mesh;
}
/**/

void MeshObject::SetModel(shared_ptr<Model> model) { 
	_model = model;
}

void MeshObject::TraceNode(FbxNode *currentNode, shared_ptr<MeshObject> parent) {
	for (auto i = 0; i < currentNode->GetChildCount(); i++) {
		FbxNode *node = currentNode->GetChild(i);

		/*
		node->GetGeometricTranslation(FbxNode::eSourcePivot);
		node->GetGeometricRotation(FbxNode::eSourcePivot);
		node->GetGeometricScaling(FbxNode::eSourcePivot);
		node->EvaluateGlobalTransform().GetT();
		node->EvaluateGlobalTransform().GetR();
		node->EvaluateGlobalTransform().GetS();
		node->EvaluateLocalTransform().GetT();
		node->EvaluateLocalTransform().GetR();
		node->EvaluateLocalTransform().GetS();
		node->EvaluateLocalTranslation();
		node->EvaluateLocalRotation();
		node->EvaluateLocalScaling();
		node->LclTranslation.Get()
		node->LclRotation.Get()
		node->LclScaling.Get()
		*/

		shared_ptr<MeshObject> meshObj = make_shared<MeshObject>(_renderer);
		meshObj->name = node->GetName();
		meshObj->parentNode = parent; 
		meshObj->transform.position = FbxVector2XMVector(node->EvaluateLocalTransform().GetT());
		meshObj->transform.rotation = FbxVector2XMVector(node->EvaluateLocalTransform().GetR());
		meshObj->transform.scale *= FbxVector2XMVector(node->EvaluateLocalTransform().GetS());

		childNodes.push_back(meshObj);

		if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
			auto fbxMesh = (FbxMesh *) node->GetNodeAttribute();

			/*auto model = make_shared<Model>();
			model->LoadMesh(fbxMesh);
			meshObj->SetModel(model);
			/**/
			vector<VertexBuffer> mesh;
			for (auto faceIndex = 0; faceIndex < fbxMesh->GetPolygonCount(); faceIndex++) {
				int polySize = fbxMesh->GetPolygonSize(faceIndex);
				assert(polySize == 3);

				int triIndex[3];
				int vindex[3] = {0 ,2, 1};
				for (auto vertexIndex = 0; vertexIndex < polySize; vertexIndex++) {
					XMVECTOR vertex = FbxVector2XMVector(fbxMesh->GetControlPointAt(fbxMesh->GetPolygonVertex(faceIndex, vindex[vertexIndex])));

					FbxVector4 fbxNormal;
					fbxMesh->GetPolygonVertexNormal(faceIndex, vindex[vertexIndex], fbxNormal);
					XMVECTOR normal = FbxVector2XMVector(fbxNormal);

					
					FbxVector2 fbxUV;
					bool ummaped;
					FbxStringList uvNames;
					fbxMesh->GetUVSetNames(uvNames);
					fbxMesh->GetPolygonVertexUV(faceIndex, vindex[vertexIndex], uvNames[0], fbxUV, ummaped);
					XMFLOAT2 uv = XMFLOAT2(fbxUV.mData[0], fbxUV.mData[1]);

					triIndex[vindex[vertexIndex]] = mesh.size();
					mesh.push_back({vertex, normal, XMVectorZero(), uv});
				}

				VertexBuffer vbs[3] = {
					mesh[triIndex[0]],
					mesh[triIndex[1]],
					mesh[triIndex[2]]
				};
				
				XMVECTOR posV[2] = {
					vbs[1].position - vbs[0].position,
					vbs[2].position - vbs[0].position
				};
				
				XMFLOAT2 uvV[2] = {
					XMFLOAT2(vbs[1].uv.x - vbs[0].uv.x, vbs[1].uv.y - vbs[0].uv.y),
					XMFLOAT2(vbs[2].uv.x - vbs[0].uv.x, vbs[2].uv.y - vbs[0].uv.y)
				};

				float f = 1.0f / (uvV[0].x * uvV[1].y - uvV[1].x * uvV[0].y);

				mesh[triIndex[0]].tangent =
					XMVectorSet(
						f * (uvV[1].y * posV[0].m128_f32[0] - uvV[0].y * posV[1].m128_f32[0]),
						f * (uvV[1].y * posV[0].m128_f32[1] - uvV[0].y * posV[1].m128_f32[1]),
						f * (uvV[1].y * posV[0].m128_f32[2] - uvV[0].y * posV[1].m128_f32[2]),
						0.0f
					);
				mesh[triIndex[1]].tangent = mesh[triIndex[0]].tangent;
				mesh[triIndex[2]].tangent = mesh[triIndex[0]].tangent;
			}
			meshObj->SetMesh(mesh);
			/**/

			_renderer->AddRenderObject(meshObj);
		}

		if (node->GetChildCount() > 0) {
			TraceNode(node, meshObj);
		}
	}
}

FbxNode *MeshObject::LoadFbx(const char *filename) {
	FbxNode *rootNode;

	FbxManager *fbxManager = FbxManager::Create();
	FbxIOSettings *ioSetting = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ioSetting);

	FbxImporter *importer = FbxImporter::Create(fbxManager, "");

	if (!importer->Initialize(filename, -1, fbxManager->GetIOSettings())) {
		OutputDebugString(L"Call to FbxImporter::Initialize() failed.\n");
		OutputDebugString((LPCWSTR) ("Error returned: " + string(importer->GetStatus().GetErrorString()) + "\n").data());
		exit(-1);
	}
	
	FbxScene *scene = FbxScene::Create(fbxManager, "myScene");
	importer->Import(scene);
	importer->Destroy();

	rootNode = scene->GetRootNode();
	TraceNode(rootNode, nullptr);

	fbxManager->Destroy();

	return rootNode;
}

