#pragma
#include "../../Core/RenderingEngine.h"
#include "../../../../Core/World.h"
#include "../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../Manage/LightManage.h"
#include "../../../../Rendering/Core/DirectX/RenderingPipeline/DescriptorHeap/DirectXDescriptorHeap.h"

class CDirectXRenderingEngine :public CRenderingEngine
{
	friend class IDirectXDeviceInterface;
public:
	CDirectXRenderingEngine();
	virtual int PreInit(FWinMainCommandParameters& InParameters) override;//��д����Ĵ��麯��
	virtual int Init(FWinMainCommandParameters InParameters) override;//��д����Ĵ��麯��
	virtual int PostInit() override;//��д����Ĵ��麯��
	virtual void Tick(float DeltaTime) override;//��д����Ĵ��麯��
	virtual int PreExit() override;//��д����Ĵ��麯��
	virtual int Exit() override;//��д����Ĵ��麯��
	virtual int PostExit() override;//��д����Ĵ��麯��
	virtual void UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo) override;//��д������麯��

public:
	void StartSetMainViewportRenderTarget();// ��ʼ�������ӿڵ�rendertarget
	void EndSetMainViewportRenderTarget();// �����������ӿڵ�rendertarget
	void ClearMainSwapChain();// ������ӿڵĽ�����

public:
	//��ȡ��������
	ComPtr<ID3D12Device> GetD3dDevice() const;
	//��ȡ�����б�
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const;
	//��ȡ���������
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	//��ȡ��ǰ�ĺ󻺳�������
	DXGI_FORMAT GetBackBufferFormat() const;
	//��ȡ��ǰ�����ģ��������
	DXGI_FORMAT GetDepthStencilFormat() const;
	//��ȡ��ǰ�����ģ��������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDSBufferView() const;
	//��ȡ��ǰ����Դ������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;
	//��ȡ�������е�buffer
	ID3D12Resource* GetCurrentSwapBuffer() const;
	//��ȡ��ǰ��������
	UINT GetDXGISampleCount() const;
	//��ȡ��ǰ��������
	UINT GetDXGISampleQuality() const;
	//��������
	void SetWorld(CWorld* InWorld);
	//��ȡ����
	CWorld* GetWorld() const;
private:
	//�ȴ�gpu���������ж����е�����
	void WaitGPUCommandQueueComplete();
	bool InitDirect3D();
	bool PostInitDirect3D();
private:
	ComPtr<IDXGIFactory4>	DXGIFactory;							// DX����
	ComPtr<ID3D12Device>	D3dDevice;								// DX����
	ComPtr<ID3D12Fence>		Fence;									// ��ԴΧ��������CPU��GPUͬ����
	ComPtr<ID3D12CommandQueue>			CommandQueue;				// �������
	ComPtr<ID3D12CommandAllocator>		CommandAllocator;			// ���������
	ComPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;		// �����б�
	ComPtr<IDXGISwapChain> SwapChain;								// ������
	ComPtr<ID3D12DescriptorHeap> RTVHeap;							// RTV��������
	ComPtr<ID3D12DescriptorHeap> DSVHeap;							// DSV��������
	FDirectXDescriptorHeap DescriptorHeap;							// CBV_SRV_UAV�����������ѣ��������е�cbv srv uav��Դ��Ӧ�÷������������


	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;			// �������д�ŵ�rtv���͵���Դ
	ComPtr<ID3D12Resource> DepthStencilBuffer;						// dsv���͵���Դ
	D3D12_VIEWPORT ViewPortInfo;									// Describes the dimensions of a viewport
	D3D12_RECT ViewPortRect;										// Describes the dimensions of a viewport
private:
	UINT CurrentFenceIndex;											// ��ǰ��Χ��ֵ
	int CurrentSwapBufferIndex;										// ��ǰ����������Դ����
	UINT M4XQualityLevels;											// ���ز����������ȼ�
	bool bMSAA4XEnabled;											// �Ƿ������ز���
	DXGI_FORMAT BackBufferFormat;									// ��̨������Դ��ʽ
	DXGI_FORMAT DepthStencilFormat;									// ���ģ����Դ��ʽ
	UINT RTVDescriptorSize;											// rtv��������С
private:
	CMeshManage* MeshManage;										//mesh������
	CLightManage* LightManage;										//�ƹ������
	CWorld* World;													//����
};