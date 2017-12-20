// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "libfbxsdk.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "DirectXSH.lib")
#pragma comment(lib, "DirectXTK.lib")


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows Sources
// Windows Source Files:
#include <windows.h>

#include <D3D11.h>
#include <DXGI1_3.h>
#include <DirectXMath.h>
#include <wrl.h>

// C RunTime Source Files
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <exception>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <filesystem>
#include <regex>
#include <cmath>
#include <typeinfo> 

#include <fbxsdk.h>
#include <DirectXTex.h>
#include <DirectXSH.h>
#include <Keyboard.h>
#include <Mouse.h>


#include "Win32App.h"
#include "Scene.h"
#include "SceneObject.h"
#include "RenderObject.h"
#include "MeshObject.h"
#include "Renderer.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"
#include "SphericalHarmonicLight.h"
#include "PlayerController.h"
#include "Model.h"
#include "Texture.h"

#include "Shader.h"
#include "AbsShaderCreator.h"
#include "VertexShaderCreator.h"
#include "PixelShaderCreator.h"
