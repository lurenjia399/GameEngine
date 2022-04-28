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
	//创建常量缓冲区描述堆
	D3D12_DESCRIPTOR_HEAP_DESC CBVDescriptorHeapDesc = {};
	CBVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	CBVDescriptorHeapDesc.NumDescriptors = 2;
	CBVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	CBVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&CBVDescriptorHeapDesc, IID_PPV_ARGS(&CBVHeap)));

	//常量缓冲区的构建
	objectConstants = make_shared<FRenderingResourcesUpdate>();
	objectConstants->Init(D3dDevice.Get(), sizeof(FObjectTransformation), 1);

	D3D12_GPU_VIRTUAL_ADDRESS objadd = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
	CBVDesc.BufferLocation = objadd;
	CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();
	D3dDevice->CreateConstantBufferView(&CBVDesc, CBVHeap->GetCPUDescriptorHandleForHeapStart());
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
		//创建根签名
	CD3DX12_ROOT_PARAMETER RootParam[2];
	//对象的的descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;
	DescriptorRangeObjCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//viewport的descriptorRange
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;
	DescriptorRangeViewportCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeObjCBV);
	RootParam[1].InitAsDescriptorTable(1, &DescriptorRangeViewportCBV);

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		2, RootParam, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> errorSerializeRootSignature;
	D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		SerializeRootSignature.GetAddressOf(),
		errorSerializeRootSignature.GetAddressOf());
	if (errorSerializeRootSignature != nullptr)
	{
		Engine_Log_Error("root signature error = %s", (char*)errorSerializeRootSignature->GetBufferPointer());
		return;
	}
	ANALYSIS_HRESULT(D3dDevice->CreateRootSignature(
		0,
		SerializeRootSignature->GetBufferPointer(),
		SerializeRootSignature->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature)));
//----------根签名的创建结束-----
//----------shader的创建开始-----
	VertexShader.BuildShader(L"../LurenjiaEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
	PixelShader.BuildShader(L"../LurenjiaEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
	InputElementDesc = {
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
//----------shader的创建结束-----
	VertexStrideInBytes = sizeof(FVertex);
	IndexSize = InRenderingData->IndexData.size();
	//获取顶点数据和索引的大小
	VertexSizeInBytes = InRenderingData->VertexData.size() * VertexStrideInBytes;
	IndexSizeInBytes = IndexSize * sizeof(uint16_t);

	//在cpu中创建资源缓冲区（目前这段没有用）
	ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);
	ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

	//在gpu中创建资源缓冲区
	GPUVertexBufferPtr = ConstructGPUDefaultBuffer(VertexBufferTempPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
	GPUIndexBufferPtr = ConstructGPUDefaultBuffer(IndexBufferTempPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FMesh::BuildMesh discover error");
		return;
	}
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
