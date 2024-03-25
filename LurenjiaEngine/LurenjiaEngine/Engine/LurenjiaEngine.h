#pragma once
#pragma comment(lib, "UnLessLibrary.lib")

#include "string"
using namespace std;

#include "simple_library/public/simple_library.h"
#include "Debug/EngineDebug.h"

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_dx12.h"
#include "../imgui/backends/imgui_impl_win32.h"

#include <d3d12.h>
#include <tchar.h>

#include <vector>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>
#include <dxgi1_4.h>
#include "Rendering/DirectX12/d3dx12.h"
#include "Rendering/DirectX12/DDSTextureLoader.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include "Debug/Log/SimpleLog.h" // 这个头文件需要在 imgui.h 下边


using namespace Microsoft::WRL;
using namespace DirectX;

extern shared_ptr<class CEngine> Engine;