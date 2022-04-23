#include "ConstructBuffer.h"
#include "../../../Interface/DirectXDeviceInterface.h"


namespace ConstructBuffer
{
	ComPtr<ID3D12Resource> FConstructBuffer::ConstructGPUDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize)
	{
		ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
		if (D3dDevice == nullptr)
		{
			Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer D3dDevice is nullptr");
			return nullptr;
		}

		ComPtr<ID3D12Resource> Buffer;//���ؽ��
		D3D12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
		D3D12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		//��Buffer��Դ�ύ��defaultHeap��
		ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
			&HeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(Buffer.GetAddressOf())));

		//��OutTempBuffer��Դ�ύ��UploadHeap��
		D3D12_HEAP_PROPERTIES UpdataBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ANALYSIS_HRESULT(D3dDevice->CreateCommittedResource(
			&UpdataBufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,//generic ͨ�õ�
			nullptr,
			IID_PPV_ARGS(OutTempBuffer.GetAddressOf())));

		D3D12_SUBRESOURCE_DATA SubRresourceData = {};
		SubRresourceData.pData = InData;						//subResource ������
		SubRresourceData.RowPitch = InDataSize;					//subResource �����ݴ�С
		SubRresourceData.SlicePitch = SubRresourceData.RowPitch;//subResource ����ȼ��

		ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
		if (GraphicsCommandList == nullptr)
		{
			Engine_Log_Error("IRenderingInterface::ConstructDefaultBuffer GraphicsCommandList is nullptr");
			return nullptr;
		}
		//�ı�Buffer��Դ��״̬
		D3D12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		GraphicsCommandList->ResourceBarrier(1, &CopyDestBarrier);

		//�����ݷŵ�Buffer��
		UpdateSubresources<1>(
			GraphicsCommandList.Get(),	//�����б�
			Buffer.Get(),				//Ŀ�껺����
			OutTempBuffer.Get(),		//�м仺�����������ϴ����У�
			0,							//�м仺�����е���Դƫ��
			0,							//��Դ�е�һ������Դ����
			1,							//��Դ������Դ������
			&SubRresourceData);			//����Դ

		//�ı�Buffer��Դ��״̬
		D3D12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		GraphicsCommandList->ResourceBarrier(1, &ReadDestBarrier);

		return Buffer;
	}
}