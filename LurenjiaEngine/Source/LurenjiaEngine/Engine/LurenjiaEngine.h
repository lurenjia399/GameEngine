#pragma once
#pragma comment(lib, "UnLessLibrary.lib")

// https://www.cnblogs.com/TenosDoIt/p/3203137.html 这个文章讲的很细了
// 静态库会生成.lib文件，我们需要下面这样导入.lib。
// 动态库会生成.lib和.dll文件，我们也需要下面这样导入.lib，动态库的lib和静态库的lib不同。
// 在这里，我们都用动态库的这种
#pragma comment(lib, "LurenjiaEngineCore.lib")
#pragma comment(lib, "LurenjiaModelTool.lib")

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

#include "Path/EnginePath.h"
#include "FBX/FBXMain.h"

using namespace Microsoft::WRL;
using namespace DirectX;

extern shared_ptr<class CEngine> Engine;