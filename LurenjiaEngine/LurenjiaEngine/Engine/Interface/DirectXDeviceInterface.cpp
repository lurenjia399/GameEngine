#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

void IDirectXDeviceInterface::StartSetMainViewportRenderTarget()
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        WindowsEngine->GetRenderingEngine()->StartSetMainViewportRenderTarget();
        Engine_Log_Success("excute  StartSetMainViewportRenderTarget success")
    }
}

void IDirectXDeviceInterface::EndSetMainViewportRenderTarget()
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        WindowsEngine->GetRenderingEngine()->EndSetMainViewportRenderTarget();
        Engine_Log_Success("excute  EndSetMainViewportRenderTarget success")
    }
}

void IDirectXDeviceInterface::ClearMainSwapChain()
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        WindowsEngine->GetRenderingEngine()->ClearMainSwapChain();
        Engine_Log_Success("excute  ClearMainSwapChain success")
    }
}

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

CLightManage* IDirectXDeviceInterface::GetLightManage() const
{
    if (Engine != nullptr)
    {
        CWindowsEngine* WindowsEngine = dynamic_cast<CWindowsEngine*>(Engine);
        if (WindowsEngine != nullptr)
        {
            return WindowsEngine->GetRenderingEngine()->LightManage;//这里定义了友元，所以能访问到
        }
        return nullptr;
    }
    return nullptr;
}

ComPtr<ID3D12DescriptorHeap> IDirectXDeviceInterface::GetRTVHeap() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->RTVHeap;
    }
    return nullptr;
}

ComPtr<ID3D12DescriptorHeap> IDirectXDeviceInterface::GetDSVHeap() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->DSVHeap;
    }
    return nullptr;
}

FDirectXDescriptorHeap* IDirectXDeviceInterface::GetCBV_SRV_UAVHeap() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return &WindowsEngine->GetRenderingEngine()->DescriptorHeap;
    }
    return {};
}

UINT IDirectXDeviceInterface::GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE InDescriptorHeapType) const
{
    return GetD3dDevice()->GetDescriptorHandleIncrementSize(InDescriptorHeapType);
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

CRenderingEngine* IDirectXDeviceInterface::GetRenderEngine() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine();
    }
    return nullptr;
}

#if (EDITOR_ENGINE == 1)
CEditorEngine* IDirectXDeviceInterface::GetEditorEngine() const
{
    CWindowsEngine* WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetEditorEngine();
    }
    return nullptr;
}
#endif

void IDirectXDeviceInterface_struct::StartSetMainViewportRenderTarget()
{
    DXDeviceInterface.StartSetMainViewportRenderTarget();
}

void IDirectXDeviceInterface_struct::EndSetMainViewportRenderTarget()
{
    DXDeviceInterface.EndSetMainViewportRenderTarget();
}

void IDirectXDeviceInterface_struct::ClearMainSwapChain()
{
    DXDeviceInterface.ClearMainSwapChain();
}

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

CLightManage* IDirectXDeviceInterface_struct::GetLightManage() const
{
    return DXDeviceInterface.GetLightManage();
}

ComPtr<ID3D12DescriptorHeap> IDirectXDeviceInterface_struct::GetRTVHeap() const
{
    return DXDeviceInterface.GetRTVHeap();
}

ComPtr<ID3D12DescriptorHeap> IDirectXDeviceInterface_struct::GetDSVHeap() const
{
    return DXDeviceInterface.GetDSVHeap();
}

FDirectXDescriptorHeap* IDirectXDeviceInterface_struct::GetCBV_SRV_UAVHeap() const
{
    return DXDeviceInterface.GetCBV_SRV_UAVHeap();
}

UINT IDirectXDeviceInterface_struct::GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE InDescriptorHeapType) const
{
    return DXDeviceInterface.GetDescriptorHandleIncrementSize(InDescriptorHeapType);
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

CRenderingEngine* IDirectXDeviceInterface_struct::GetRenderEngine() const
{
    return DXDeviceInterface.GetRenderEngine();
}

#if (EDITOR_ENGINE == 1)
CEditorEngine* IDirectXDeviceInterface_struct::GetEditorEngine() const
{
    return DXDeviceInterface.GetEditorEngine();
}
#endif
