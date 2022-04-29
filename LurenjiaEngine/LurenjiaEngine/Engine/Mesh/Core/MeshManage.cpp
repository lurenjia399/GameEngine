#include "MeshManage.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../BoxMesh.h"
#include "../PlaneMesh.h"
#include "../CylinderMesh.h"
#include "../CustomMesh.h"
#include "../SphereMesh.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"

CMeshManage::CMeshManage()
	: VertexSizeInBytes(0)
	, VertexStrideInBytes(0)
	, IndexSizeInBytes(0)
	, IndexFormat(DXGI_FORMAT_R16_UINT)
	, IndexSize(0)
	, WorldMatrix(EngineMath::IdentityMatrix4x4())
	//	1,0,0,0,
	//	0,1,0,0,
	//	0,0,1,-5,
	//	0,0,0,1
	//)
{
}

void CMeshManage::Init()
{
	
}

void CMeshManage::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	RenderingPipeline.BuildPipeline();

	return;
//----------object�����������Ĵ�����ʼ-----
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh D3dDevice is nullptr");
		return;
	}

//----------object�����������Ĵ�������-----
	//viewport�����������Ĺ���
	viewportConstants = make_shared<FRenderingResourcesUpdate>();	//����������������Դ
	viewportConstants->Init(D3dDevice.Get(), sizeof(FViewportTransformation), 1);

	D3D12_GPU_VIRTUAL_ADDRESS viewportobjadd = viewportConstants.get()->GetBuffer()->GetGPUVirtualAddress();
	int descriptorIndex = 1;
	UINT HandleSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CBVHeap->GetCPUDescriptorHandleForHeapStart());
	Handle.Offset(1, HandleSize);
	D3D12_CONSTANT_BUFFER_VIEW_DESC viewportDesc = {};
	viewportDesc.BufferLocation = viewportobjadd;
	viewportDesc.SizeInBytes = viewportConstants->GetConstantBufferByteSize();
	D3dDevice->CreateConstantBufferView(&viewportDesc, Handle);

//----------��ǩ���Ĵ�����ʼ-----
//----------��ǩ���Ĵ�������-----
//----------shader�Ĵ�����ʼ-----
	
//----------shader�Ĵ�������-----
	
//----------pos ��ˮ�߰󶨿�ʼ-----
	//Ǩ�Ƶ���RenderingResourcesUpdate��
//----------pos ��ˮ�߰󶨽���-----
}

void CMeshManage::PreDraw(float DeltaTime)
{
	GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get());
}

void CMeshManage::Draw(float DeltaTime)
{
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList = GetGraphicsCommandList();
	if (GraphicsCommandList == nullptr)
	{
		Engine_Log_Error("FMesh::Draw GraphicsCommandList is nullptr");
		return;
	}

	//�������б��� ���cbv�������� ����
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GraphicsCommandList->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	//�������б��� ��Ӹ�ǩ�� ����
	GraphicsCommandList->SetGraphicsRootSignature(RootSignature.Get());
	//�������б��� ��Ӷ��㻺������ ����
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = GetVertexBufferView();
	GraphicsCommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	//�������б��� ��������������� ����
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = GetIndexBufferView();
	GraphicsCommandList->IASetIndexBuffer(&IndexBufferView);
	//�������б��� ���ͼԪ���� ����
	GraphicsCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�������б��� ��ӽ�����������ӵ���ǩ���� ����
	GraphicsCommandList->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	int descriptorIndex = 1;
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CBVHeap->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(1, HandleSize);
	GraphicsCommandList->SetGraphicsRootDescriptorTable(1, Handle);

	/// <summary>
	/// [in] UINT IndexCountPerInstance,ÿ������ʵ��������������
	///	[in] UINT InstanceCount,		��Ҫ���Ƶ�ʵ������
	///	[in] UINT StartIndexLocation,	GPU��IndexBuffer��ȡ�ĵ�һ��indexλ��
	///	[in] INT  BaseVertexLocation,	��ÿ���������ƫ�ƣ��ڶ�ȡVertexBuffer֮ǰ
	///	[in] UINT StartInstanceLocation	��ÿ���������ƫ�ƣ��ڴ�VertexBuffer��ȡʵ��֮ǰ
	/// </summary>
	/// <param name="DeltaTime"></param>

	// Draw Call !!!
	GraphicsCommandList->DrawIndexedInstanced(IndexSize, 1, 0, 0, 0);
}

void CMeshManage::PostDraw(float DeltaTime)
{
	
}

void CMeshManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	XMMATRIX MatrixWorld = XMLoadFloat4x4(&WorldMatrix);

	//object��������������ģ�ͱ任����
	FObjectTransformation ObjectTransformation;
	XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
	objectConstants->Update(0, &ObjectTransformation);
	//viewport��������������������任�����͸��ͶӰ����
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//�м���Ҫת�ã��������еľ����޷������еľ���
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//ע��constBuffer�ж�ȡ�����ǰ����ж�ȡ��
	viewportConstants->Update(0, &ViewportTransformation);
}

CMesh* CMeshManage::CreateBoxMesh(string InName, const float& Inheight, const float& Inwidth, const float& Indepth)
{
	return CreateMesh<CBoxMesh>(InName, Inheight, Inwidth, Indepth);
}

CMesh* CMeshManage::CreatePlaneMesh(string InName, const float& Inheight, const float& Inwidth, const uint32_t& InHeightSubdivide, const uint32_t& InwidthSubdivide)
{
	return CreateMesh<CPlaneMesh>(InName, Inheight, Inwidth, InHeightSubdivide, InwidthSubdivide);
}

CMesh* CMeshManage::CreateCylinderMesh(string InName, const float& InTopRadius, const float& InBottomRadius, const uint32_t& InAxialSubdivision, const float& InHeight, const uint32_t& InHeightSubdivision)
{
	return CreateMesh<CCylinderMesh>(InName, InTopRadius, InBottomRadius, InAxialSubdivision, InHeight, InHeightSubdivision);
}

CMesh* CMeshManage::CreateCustomMesh(string InName, const string& InPath)
{
	return CreateMesh<CCustomMesh>(InName, InPath);
}

CMesh* CMeshManage::CreateSphereMesh(string InName, const float& InRadius, const uint32_t& InAxialSubdivision, const uint32_t& InHeightSubdivision)
{
	return CreateMesh<CSphereMesh>(InName, InRadius, InAxialSubdivision, InHeightSubdivision);
}

D3D12_VERTEX_BUFFER_VIEW CMeshManage::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV = {};
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = VertexSizeInBytes;
	VBV.StrideInBytes = VertexStrideInBytes;

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW CMeshManage::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV = {};
	IBV.SizeInBytes = IndexSizeInBytes;
	IBV.Format = IndexFormat;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();

	return IBV;
}
