#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CWindowsEngine;
#else
class CEngien;
#endif

class IDirectXDeviceInterface
{
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

#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	CEngien* GetEngine() const;
#endif
};

struct IDirectXDeviceInterface_struct
{
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

#if defined(_WIN32)
	CWindowsEngine* GetEngine() const;
#else
	Engien* GetEngine() const;
#endif
private:
	IDirectXDeviceInterface DXDeviceInterface;
};