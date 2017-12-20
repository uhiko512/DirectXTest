#include "stdafx.h"
#include "Model.h"

bool Model::_isInputLayoutCreated = false;
bool Model::_isInputLayoutSetted = false;

Model::Model() { }

void Model::OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) {

	// Input Layout
	if (!_isInputLayoutCreated) {
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[]{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURE" , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		vector<byte> vertexShaderCode = Shader::ReadShaderByteCode("InputLayoutVS.cso");
		Renderer::ThrowIfFailed(
			d3dDevice->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderCode.data(), vertexShaderCode.size(), &_inputLayout)
		);

		_isInputLayoutCreated = true;
	}

	// Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = sizeof(VertexBuffer) * _mesh.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA verticesData = { 0 };
	verticesData.pSysMem = _mesh.data();

	Renderer::ThrowIfFailed(
		d3dDevice->CreateBuffer(&bufferDesc, &verticesData, &_vertexBuffer)
	);
}

void Model::OnRender(ComPtr<ID3D11DeviceContext> d3dContext) {
	
	if (!_isInputLayoutSetted) {
		UINT stride = sizeof(VertexBuffer);
		UINT offset = 0;

		d3dContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);

		_isInputLayoutSetted = true;
	}

	d3dContext->Draw(_mesh.size(), 0);

	OutputDebugStringA(to_string(_mesh.size()).c_str());
	OutputDebugString(L"\n");
}

void Model::LoadMesh(FbxMesh* fbxMesh) {
	//vector<VertexBuffer> mesh;

	for (auto faceIndex = 0; faceIndex < fbxMesh->GetPolygonCount(); faceIndex++) {
		int polySize = fbxMesh->GetPolygonSize(faceIndex);
		assert(polySize == 3);

		int triIndex[3];
		int vindex[3] = { 0 ,2, 1 };
		for (auto vertexIndex = 0; vertexIndex < polySize; vertexIndex++) {
			// Vertex
			XMVECTOR vertex = MeshObject::FbxVector2XMVector(fbxMesh->GetControlPointAt(fbxMesh->GetPolygonVertex(faceIndex, vindex[vertexIndex])));

			// Normal
			FbxVector4 fbxNormal;
			fbxMesh->GetPolygonVertexNormal(faceIndex, vindex[vertexIndex], fbxNormal);
			XMVECTOR normal = MeshObject::FbxVector2XMVector(fbxNormal);

			// UV
			bool ummaped;
			FbxVector2 fbxUV;
			FbxStringList uvNames;
			fbxMesh->GetUVSetNames(uvNames);
			fbxMesh->GetPolygonVertexUV(faceIndex, vindex[vertexIndex], uvNames[0], fbxUV, ummaped);
			XMFLOAT2 uv = XMFLOAT2(fbxUV.mData[0], fbxUV.mData[1]);

			triIndex[vindex[vertexIndex]] = _mesh.size();
			_mesh.push_back({ vertex, normal, XMVectorZero(), uv });
		}

		// Tangent
		VertexBuffer vbs[3] = {
			_mesh[triIndex[0]],
			_mesh[triIndex[1]],
			_mesh[triIndex[2]]
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

		_mesh[triIndex[0]].tangent =
			XMVectorSet(
				f * (uvV[1].y * posV[0].m128_f32[0] - uvV[0].y * posV[1].m128_f32[0]),
				f * (uvV[1].y * posV[0].m128_f32[1] - uvV[0].y * posV[1].m128_f32[1]),
				f * (uvV[1].y * posV[0].m128_f32[2] - uvV[0].y * posV[1].m128_f32[2]),
				0.0f
			);
		_mesh[triIndex[1]].tangent = _mesh[triIndex[0]].tangent;
		_mesh[triIndex[2]].tangent = _mesh[triIndex[0]].tangent;
	}

	//_mesh = mesh;
}

