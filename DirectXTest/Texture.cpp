#include "stdafx.h"
#include "Texture.h"


Texture::Texture(string name) {
	regex texName(name + "_(.+)\\.tga");

	OutputDebugStringA(("\n\n" + name + "\n\n").c_str());
	for (auto& p : std::experimental::filesystem::directory_iterator("./Texture")) {
		string s = p.path().filename().generic_string();
		smatch strMatch;
		if (regex_match(s, strMatch, texName)) {
			OutputDebugStringA(s.c_str());
			OutputDebugStringA("\n");
			OutputDebugStringA(strMatch[1].str().c_str());
			OutputDebugStringA("\n");
		}
	}
}

void Texture::OnInitRenderer(ComPtr<ID3D11Device> d3dDevice) { 

}

void Texture::OnRender(ComPtr<ID3D11DeviceContext> d3dContext) { 

}

