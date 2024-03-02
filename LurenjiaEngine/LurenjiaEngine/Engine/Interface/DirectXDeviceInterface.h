#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CWindowsEngine;
class CMeshManage;
class CLightManage;
class CRenderingEngine;
struct FDirectXDescriptorHeap;
#else
class CEngien;
#endif

#if (EDITOR_ENGINE == 1)
class CEditorEngine;
#endif

class IDirectXDeviceInterface
{
public:
	void StartSetMainViewportRenderTarget();// ��ʼ�������ӿڵ�rendertarget
	void EndSetMainViewportRenderTarget();// �����������ӿڵ�rendertarget
	void ClearMainSwapChain();// ������ӿڵĽ�����
public:
	//��ȡ��ǰΧ��
	ComPtr<ID3D12Fence> GetFence() const;
	//��ȡ��������
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//��ȡ�����б�
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//��ȡ���������
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//��ȡ�������
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	//��ȡwindowsHandle
	HWND GetMainWindowsHandle() const;
	//��ȡmesh������
	shared_ptr<CMeshManage> GetMeshManage() const;
	//��ȡlight������
	shared_ptr<CLightManage> GetLightManage() const;
	//��ȡRTV��������
	ComPtr<ID3D12DescriptorHeap> GetRTVHeap() const;
	//��ȡDSV��������
	ComPtr<ID3D12DescriptorHeap> GetDSVHeap() const;
	//��ȡCBV_SRV_UAV��������
	FDirectXDescriptorHeap* GetCBV_SRV_UAVHeap() const;
	//��ȡĳ����������С
	UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE InDescriptorHeapType) const;
	//��ȡ����
#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	CEngien* GetEngine() const;
#endif
	//��ȡ��Ⱦ����
	shared_ptr<CRenderingEngine> GetRenderEngine() const;
	
	//��ȡ�༭������
#if (EDITOR_ENGINE == 1)
	shared_ptr<CEditorEngine> GetEditorEngine() const;
#endif
	
};

struct IDirectXDeviceInterface_struct
{
	// ��ʼ�������ӿڵ�rendertarget
	void StartSetMainViewportRenderTarget();
	// �����������ӿڵ�rendertarget
	void EndSetMainViewportRenderTarget();
	// ������ӿڵĽ�����
	void ClearMainSwapChain();


	//��ȡ��ǰΧ��
	ComPtr<ID3D12Fence> GetFence() const;
	//��ȡ��������
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//��ȡ�����б�
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//��ȡ���������
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//��ȡ�������
	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	//��ȡwindowsHandle
	HWND GetMainWindowsHandle() const;
	//��ȡmesh������
	shared_ptr<CMeshManage> GetMeshManage() const;
	//��ȡlight������
	shared_ptr<CLightManage> GetLightManage() const;
	//��ȡRTV��������
	ComPtr<ID3D12DescriptorHeap> GetRTVHeap() const;
	//��ȡDSV��������
	ComPtr<ID3D12DescriptorHeap> GetDSVHeap() const;
	//��ȡCBV_SRV_UAV��������
	FDirectXDescriptorHeap* GetCBV_SRV_UAVHeap() const;
	//��ȡĳ����������С
	UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE InDescriptorHeapType) const;
#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	Engien* GetEngine() const;
#endif
	//��ȡ��Ⱦ����
	shared_ptr<CRenderingEngine> GetRenderEngine() const;

	//��ȡ�༭������
#if (EDITOR_ENGINE == 1)
	shared_ptr<CEditorEngine> GetEditorEngine() const;
#endif
	
private:
	IDirectXDeviceInterface DXDeviceInterface;
};
