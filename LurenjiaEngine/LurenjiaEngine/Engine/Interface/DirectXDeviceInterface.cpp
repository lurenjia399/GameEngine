#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"
#include "../Rendering/Core/DirectX/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

void IDirectXDeviceInterface::StartSetMainViewportRenderTarget()
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        WindowsEngine->GetRenderingEngine()->StartSetMainViewportRenderTarget();
        Engine_Log_Success("excute  StartSetMainViewportRenderTarget success")
    }
}

void IDirectXDeviceInterface::EndSetMainViewportRenderTarget()
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        WindowsEngine->GetRenderingEngine()->EndSetMainViewportRenderTarget();
        Engine_Log_Success("excute  EndSetMainViewportRenderTarget success")
    }
}

void IDirectXDeviceInterface::ClearMainSwapChain()
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        WindowsEngine->GetRenderingEngine()->ClearMainSwapChain();
        //Engine_Log_Success("excute  ClearMainSwapChain success")
    }
}

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->Fence;
    }
    return nullptr;
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->D3dDevice;
    }
    return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->GraphicsCommandList;
    }
    return nullptr;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->CommandAllocator;
    }
    return nullptr;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->CommandQueue;
    }
    return nullptr;
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->MainWindowsHandle;
    }
    return nullptr;
}

shared_ptr<CMeshManage> IDirectXDeviceInterface::GetMeshManage() const
{
    if (Engine != nullptr)
    {
        shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
        if (WindowsEngine.use_count() != 0)
        {
            return WindowsEngine->GetRenderingEngine()->MeshManage;//这里定义了友元，所以能访问到
        }
        return nullptr;
    }
    return nullptr;
}

shared_ptr<CLightManage> IDirectXDeviceInterface::GetLightManage() const
{
    if (Engine != nullptr)
    {
        shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
        if (WindowsEngine.use_count() != 0)
        {
            return WindowsEngine->GetRenderingEngine()->LightManage;//这里定义了友元，所以能访问到
        }
        return nullptr;
    }
    return nullptr;
}

ComPtr<ID3D12DescriptorHeap> IDirectXDeviceInterface::GetRTVHeap() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->RTVHeap;
    }
    return nullptr;
}

ComPtr<ID3D12DescriptorHeap> IDirectXDeviceInterface::GetDSVHeap() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine()->DSVHeap;
    }
    return nullptr;
}

FDirectXDescriptorHeap* IDirectXDeviceInterface::GetCBV_SRV_UAVHeap() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
    if (WindowsEngine.use_count() != 0 && WindowsEngine->GetRenderingEngine())
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
shared_ptr<CWindowsEngine> IDirectXDeviceInterface::GetEngine() const
{
    if (Engine != nullptr)
    {
        shared_ptr<CWindowsEngine> WindowsEngine = static_pointer_cast<CWindowsEngine>(Engine);
        if (WindowsEngine.use_count() != 0)
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

shared_ptr<CRenderingEngine> IDirectXDeviceInterface::GetRenderEngine() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
    if (WindowsEngine && WindowsEngine->GetRenderingEngine())
    {
        return WindowsEngine->GetRenderingEngine();
    }
    return nullptr;
}

shared_ptr<CWorld> IDirectXDeviceInterface::GetWorld() const
{
    auto renderengine = GetRenderEngine();
    if(renderengine.use_count() > 0)
    {
        auto dxRenderEngine = static_pointer_cast<CDirectXRenderingEngine>(renderengine);
        if(dxRenderEngine.use_count() > 0)
        {
            return dxRenderEngine->GetWorld();
        }
    }
    return nullptr;
}

#if (EDITOR_ENGINE == 1)
shared_ptr<CEditorEngine> IDirectXDeviceInterface::GetEditorEngine() const
{
    shared_ptr<CWindowsEngine> WindowsEngine = GetEngine();
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

shared_ptr<CMeshManage> IDirectXDeviceInterface_struct::GetMeshManage() const
{
    return DXDeviceInterface.GetMeshManage();
}

shared_ptr<CLightManage> IDirectXDeviceInterface_struct::GetLightManage() const
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
shared_ptr<CWindowsEngine> IDirectXDeviceInterface_struct::GetEngine() const
{
    return DXDeviceInterface.GetEngine();
}


#else
CEngien* IDirectXDeviceInterface_struct::GetEngine() const
{
    return DXDeviceInterface.GetEngine();
}
#endif

shared_ptr<CRenderingEngine> IDirectXDeviceInterface_struct::GetRenderEngine() const
{
    return DXDeviceInterface.GetRenderEngine();
}

shared_ptr<CWorld> IDirectXDeviceInterface_struct::GetWorld() const
{
    return DXDeviceInterface.GetWorld();
}

#if (EDITOR_ENGINE == 1)
shared_ptr<CEditorEngine> IDirectXDeviceInterface_struct::GetEditorEngine() const
{
    return DXDeviceInterface.GetEditorEngine();
}
#endif
