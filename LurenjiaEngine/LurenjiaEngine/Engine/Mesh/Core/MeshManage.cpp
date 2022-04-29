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
//----------object常量缓冲区的创建开始-----
	ComPtr<ID3D12Device> D3dDevice = GetD3dDevice();
	if (D3dDevice == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh D3dDevice is nullptr");
		return;
	}

//----------object常量缓冲区的创建结束-----
	//viewport常量缓冲区的构建
	viewportConstants = make_shared<FRenderingResourcesUpdate>();	//创建常量缓冲区资源
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

//----------根签名的创建开始-----
//----------根签名的创建结束-----
//----------shader的创建开始-----
	
//----------shader的创建结束-----
	
//----------pos 流水线绑定开始-----
	//迁移到了RenderingResourcesUpdate中
//----------pos 流水线绑定结束-----
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

	//向命令列表中 添加cbv描述符堆 命令
	ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
	GraphicsCommandList->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
	//向命令列表中 添加根签名 命令
	GraphicsCommandList->SetGraphicsRootSignature(RootSignature.Get());
	//向命令列表中 添加顶点缓冲数据 命令
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView = GetVertexBufferView();
	GraphicsCommandList->IASetVertexBuffers(0, 1, &VertexBufferView);
	//向命令列表中 添加索引缓冲数据 命令
	D3D12_INDEX_BUFFER_VIEW IndexBufferView = GetIndexBufferView();
	GraphicsCommandList->IASetIndexBuffer(&IndexBufferView);
	//向命令列表中 添加图元拓扑 命令
	GraphicsCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//向命令列表中 添加将描述符表添加到根签名中 命令
	GraphicsCommandList->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

	int descriptorIndex = 1;
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CBVHeap->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(1, HandleSize);
	GraphicsCommandList->SetGraphicsRootDescriptorTable(1, Handle);

	/// <summary>
	/// [in] UINT IndexCountPerInstance,每个绘制实例中索引的数量
	///	[in] UINT InstanceCount,		需要绘制的实例数量
	///	[in] UINT StartIndexLocation,	GPU从IndexBuffer读取的第一个index位置
	///	[in] INT  BaseVertexLocation,	给每个索引添加偏移，在读取VertexBuffer之前
	///	[in] UINT StartInstanceLocation	给每个索引添加偏移，在从VertexBuffer读取实例之前
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

	//object常量缓冲区传入模型变换矩阵
	FObjectTransformation ObjectTransformation;
	XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
	objectConstants->Update(0, &ObjectTransformation);
	//viewport常量缓冲区传入摄像机变换矩阵和透视投影矩阵
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//切记需要转置，，，主列的矩阵无法乘主行的矩阵
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//注意constBuffer中读取矩阵是按照列读取的
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
