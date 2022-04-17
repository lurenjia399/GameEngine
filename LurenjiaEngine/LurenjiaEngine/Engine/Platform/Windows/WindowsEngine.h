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

	virtual void Tick(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();
//----------interface-----
public:
	//��ȡ��������
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//��ȡ�����б�
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//��ȡ���������
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//��ȡ�������е�buffer
	ID3D12Resource* GetCurrentSwapBuffer() const;
	//��ȡ��ǰ����Դ������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
	//��ȡ��ǰ�����ģ��������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDSBufferView() const;
	//��ȡ��ǰ�ĺ󻺳�������
	DXGI_FORMAT GetBackBufferFormat() const;
	//��ȡ��ǰ�����ģ��������
	DXGI_FORMAT GetDepthStencilFormat() const;
	//��ȡ��ǰ��������
	UINT GetDXGISampleCount() const;
	//��ȡ��ǰ��������
	UINT GetDXGISampleQuality() const;
protected:
	//�ȴ�gpu���������ж����е�����
	void WaitGPUCommandQueueComplete();
//----------interface-----
private:
	bool InitWindows(FWinMainCommandParameters InParameters);
	bool InitDirect3D();
	bool PostInitDirect3D();
protected:

	UINT CurrentFenceIndex;					//��ǰ��Χ��ֵ
	int CurrentSwapBufferIndex;				//��ǰ����������ʹ�õĻ�����index

	ComPtr<IDXGIFactory4>	DXGIFactory;	// ���� DirectX ͼ�λ����ṹ��DXGI������
	ComPtr<ID3D12Device>	D3dDevice;		// ���� ����������������б�������У�Fence, ��Դ���ܵ�״̬���󣬶�
	ComPtr<ID3D12Fence>		Fence;

	ComPtr<ID3D12CommandQueue>			CommandQueue;
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;

	ComPtr<IDXGISwapChain> SwapChain;

	//������������
	ComPtr<ID3D12DescriptorHeap> RTVHeap;//(running target view) ��ȾĿ������
	ComPtr<ID3D12DescriptorHeap> DSVHeap;//(depth stencil view) ��ȾĿ�����/ģ�干������

	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;//��������Դ
	ComPtr<ID3D12Resource> DepthStencilBuffer;			//���ģ����Դ

	D3D12_VIEWPORT ViewPortInfo;
	D3D12_RECT ViewPortRect;
protected:
	HWND MyWindowsHandle;		//�����ھ��
	UINT M4XQualityLevels;		//���ز����������ȼ�
	bool bMSAA4XEnabled;		//���ز����Ƿ���
	DXGI_FORMAT BackBufferFormat;	//��̨��������ʽ
	DXGI_FORMAT DepthStencilFormat;	//���ģ���ʽ
	UINT RTVDescriptorSize;		//rtv��������С
};
#endif