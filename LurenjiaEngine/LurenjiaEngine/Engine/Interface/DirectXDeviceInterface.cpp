#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->Fence;
    }
    return nullptr;
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->D3dDevice;
    }
    return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->GraphicsCommandList;
    }
    return nullptr;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->CommandAllocator;
    }
    return nullptr;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->CommandQueue;
    }
    return nullptr;
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->MainWindowsHandle;
    }
    return nullptr;
}

CMeshManage* IDirectXDeviceInterface::GetMeshManage() const
{
    if (Engine != nullptr)
    {
        CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
        if (WindowsEngine != nullptr)
        {
            return WindowsEngine->GetRenderingEngine()->MeshManage;//这里定义了友元，所以能访问到
        }
        return nullptr;
    }
    return nullptr;
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface::GetEngine() const
{
    if (Engine != nullptr)
    {
        CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
        if (WindowsEngine != nullptr)
        {
            return WindowsEngine;
        }
        return nullptr;
    }
    return nullptr;
}
#else
CEngien* IDirectXDeviceInterface::GetEngine() const
{
    return nullptr;
}
#endif

ComPtr<ID3D12Fence> IDirectXDeviceInterface_struct::GetFence() const
{
    return DXDeviceInterface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_struct::GetD3dDevice() const
{
    return DXDeviceInterface.GetD3dDevice();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_struct::GetGraphicsCommandList() const
{
    return DXDeviceInterface.GetGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_struct::GetCommandAllocator() const
{
    return DXDeviceInterface.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_struct::GetCommandQueue() const
{
    return DXDeviceInterface.GetCommandQueue();
}

HWND IDirectXDeviceInterface_struct::GetMainWindowsHandle() const
{
    return DXDeviceInterface.GetMainWindowsHandle();
}

CMeshManage* IDirectXDeviceInterface_struct::GetMeshManage() const
{
    return DXDeviceInterface.GetMeshManage();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface_struct::GetEngine() const
{
    return DXDeviceInterface.GetEngine();
}

#else
CEngien* IDirectXDeviceInterface_struct::GetEngine() const
{
    return DXDeviceInterface.GetEngine();
}
#endif
