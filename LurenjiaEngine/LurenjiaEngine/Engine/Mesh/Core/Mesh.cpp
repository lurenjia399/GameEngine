#include "Mesh.h"

FMesh::FMesh():
	VertexSizeInBytes(0),
	VertexStrideInBytes(0),
	IndexSizeInBytes(0),
	IndexFormat(DXGI_FORMAT_R16_UINT),
	IndexSize(0)
{
}

void FMesh::Init()
{
}

void FMesh::Draw(float DeltaTime)
{
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("FMesh::Draw GraphicsCommandList is nullptr");
		return;
	}
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = GetIndexBufferView();
	GraphicsCommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	GraphicsCommandList->IASetIndexBuffer(&IndexBufferView);

	//����ͼԪ��triandleList
	GraphicsCommandList->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/// <summary>
	/// [in] UINT IndexCountPerInstance,ÿ������ʵ��������������
	///	[in] UINT InstanceCount,		��Ҫ���Ƶ�ʵ������
	///	[in] UINT StartIndexLocation,	GPU��IndexBuffer��ȡ�ĵ�һ��indexλ��
	///	[in] INT  BaseVertexLocation,	��ÿ���������ƫ�ƣ��ڶ�ȡVertexBuffer֮ǰ
	///	[in] UINT StartInstanceLocation	��ÿ���������ƫ�ƣ��ڴ�VertexBuffer��ȡʵ��֮ǰ
	/// </summary>
	/// <param name="DeltaTime"></param>
	GraphicsCommandList->DrawIndexedInstanced(IndexSize, 1, 0, 0, 0);
}

void FMesh::BuildMesh(const FMeshRenderingData& InRenderingData)
{
//----------�����������Ĵ�����ʼ-----
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh D3dDevice is nullptr");
		return;
	}
	objectConstants = make_shared<FRenderingResourcesUpdate>();
	objectConstants->Init(D3dDevice.Get(), sizeof(FObjectTransformation) , 1);
	D3D12_GPU_VIRTUAL_ADDRESS objadd = objectConstants->GetBuffer()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
	CBVDesc.BufferLocation = objadd;
	CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();
	D3D12_DESCRIPTOR_HEAP_DESC CBVDescriptorHeapDesc = {};
	CBVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBVDescriptorHeapDesc.NumDescriptors = 1;
	CBVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&CBVDescriptorHeapDesc, IID_PPV_ARGS(CBVHeap.GetAddressOf())));
	D3dDevice->CreateConstantBufferView(&CBVDesc, CBVHeap->GetCPUDescriptorHandleForHeapStart());
//----------�����������Ĵ�������-----
//----------��ǩ���Ĵ�����ʼ-----
	//������ǩ��
	CD3DX12_ROOT_PARAMETER RootParam[1];
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCBV;
	DescriptorRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeCBV);

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		_countof(RootParam), RootParam, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> errorSerializeRootSignature;
	D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		SerializeRootSignature.GetAddressOf(),
		errorSerializeRootSignature.GetAddressOf());
	if (!errorSerializeRootSignature)
	{
		Engine_Log_Error("root signature error = %s", (char*)errorSerializeRootSignature->GetBufferPointer());
		return;
	}
	D3dDevice->CreateRootSignature(
		0,
		SerializeRootSignature->GetBufferPointer(),
		SerializeRootSignature->GetBufferSize(),
		IID_PPV_ARGS(RootSignature.GetAddressOf()));
//----------��ǩ���Ĵ�������-----
//----------shader�Ĵ�����ʼ-----
	VertexShader.BuildShader(L"../../../Shader/Hello.hlsl", "VertexShader", "vs_5_0");
	PixelShader.BuildShader(L"../../../Shader/Hello.hlsl", "PixelShader", "vs_5_0");
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
//----------shader�Ĵ�������-----
	IndexSize = InRenderingData.IndexData.size();
	VertexStrideInBytes = sizeof(FVertex);
	//��ȡ�������ݺ������Ĵ�С
	VertexSizeInBytes = InRenderingData.VertexData.size() * VertexStrideInBytes;
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);

	//Ϊ�������������� ���ݸ�cpuBuffer
	ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData.VertexData.data(), VertexSizeInBytes);
	ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData.IndexData.data(), IndexSizeInBytes);

	GPUVertexBufferPtr = ConstructDefaultBuffer(VertexBufferTempPtr, InRenderingData.VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructDefaultBuffer(IndexBufferTempPtr, InRenderingData.IndexData.data(), IndexSizeInBytes);
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh discover error");
		return;
	}
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV = {};
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = VertexSizeInBytes;
	VBV.StrideInBytes = VertexStrideInBytes;

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV = {};
	IBV.SizeInBytes = IndexSizeInBytes;
	IBV.Format = IndexFormat;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();

	return IBV;
}

FObjectTransformation::FObjectTransformation()
	: World(
		1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1.0f)
{
}
