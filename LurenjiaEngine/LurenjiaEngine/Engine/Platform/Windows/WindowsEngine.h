#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	FWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters& InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();
private:
	bool InitWindows(FWinMainCommandParameters InParameters);

	bool InitDirect3D();
protected:
	ComPtr<IDXGIFactory4>	DXGIFactory;	// ���� DirectX ͼ�λ����ṹ��DXGI������
	ComPtr<ID3D12Device>	D3dDevice;		// ���� ����������������б�������У�Fence, ��Դ���ܵ�״̬���󣬶�
	ComPtr<ID3D12Fence>		Fence;

	ComPtr<ID3D12CommandQueue>			CommandQueue;
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommamdList;

	ComPtr<IDXGISwapChain> SwapChain;

	//������������
	ComPtr<ID3D12DescriptorHeap> RTVHeap;//(running target view) ��ȾĿ������
	ComPtr<ID3D12DescriptorHeap> DSVHeap;//(depth stencil view) ��ȾĿ�����/ģ�干������

	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;//��������Դ
	ComPtr<ID3D12Resource> DepthStencilBuffer;			//���ģ����Դ

protected:
	HWND MyWindowsHandle;		//�����ھ��
	UINT M4XQualityLevels;		//���ز����������ȼ�
	bool bMSAA4XEnabled;		//���ز����Ƿ���
	DXGI_FORMAT BackBufferFormat;	//��̨��������ʽ
	DXGI_FORMAT DepthStencilFormat;	//���ģ���ʽ
	UINT RTVDescriptorSize;		//rtv��������С

};
#endif