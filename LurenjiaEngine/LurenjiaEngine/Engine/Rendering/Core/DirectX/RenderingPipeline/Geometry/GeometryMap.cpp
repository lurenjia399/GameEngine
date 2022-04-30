#include "GeometryMap.h"
#include "../../../../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

FGeometryMap::FGeometryMap()
{
	Geometrys.emplace(0, FGeometry());
}

void FGeometryMap::BuildMeshDescData(CMesh* InMesh, const FMeshRenderingData& InRenderingData)
{
	FGeometry& Geometry = Geometrys[0];
	Geometry.BuildMeshDescData(InMesh, InRenderingData);
}

void FGeometryMap::BuildMeshBuffer()
{
	for (pair<const int, FGeometry>& Geometry : Geometrys)
	{
		Geometry.second.BuildMeshBuffer(Geometry.first);
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1代表摄像机
	DescriptorHeap.BuildDescriptorHeap(GetDrawObjectNumber() + 1);
}

void FGeometryMap::BuildObjectConstantBufferView()
{
	ObjectConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectNumber());
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	ObjectConstantBufferView.BuildConstantBuffer(Handle, GetDrawObjectNumber(), 0);
}

UINT FGeometryMap::GetDrawObjectNumber()
{
	return Geometrys[0].GetDrawObjectNumber();
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	ViewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 1);
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	ViewportConstantBufferView.BuildConstantBuffer(Handle, 1, GetDrawObjectNumber());
}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (pair<const int, FGeometry> temp : Geometrys)
	{
		for (UINT i = 0; i < temp.second.DescribeMeshRenderingData.size(); ++i)
		{
			XMMATRIX MatrixWorld = XMLoadFloat4x4(&temp.second.DescribeMeshRenderingData[i].WorldMatrix);

			//object常量缓冲区传入模型变换矩阵
			FObjectTransformation ObjectTransformation;
			XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
			ObjectConstantBufferView.Update(i, &ObjectTransformation);
		}
	}
	//viewport常量缓冲区传入摄像机变换矩阵和透视投影矩阵
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//切记需要转置，，，主列的矩阵无法乘主行的矩阵
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//注意constBuffer中读取矩阵是按照列读取的
	ViewportConstantBufferView.Update(0, &ViewportTransformation);
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	DrawViewport(DeltaTime);
	DrawMesh(DeltaTime);
	
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (pair<const int, FGeometry>& tem : Geometrys)
	{
		D3D12_VERTEX_BUFFER_VIEW VBV = tem.second.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW IBV = tem.second.GetIndexBufferView();
		
		for (UINT i = 0; i < tem.second.DescribeMeshRenderingData.size(); ++i)
		{
			CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
			
			FGeometryDescData& data = tem.second.DescribeMeshRenderingData[0];
			//向命令列表中 添加顶点缓冲数据 命令
			GetGraphicsCommandList()->IASetVertexBuffers(0, 1, &VBV);
			//向命令列表中 添加索引缓冲数据 命令
			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
			//向命令列表中 添加图元拓扑 命令
			GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//偏移
			Handle.Offset(i, HandleSize);
			//向命令列表中 添加将描述符表添加到根签名中 命令
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, Handle);
			// Draw Call !!!
			GetGraphicsCommandList()->DrawIndexedInstanced(
				data.IndexSize,				//绘制的实例所需的索引数量
				1,							//绘制的实例个数
				data.IndexoffsetPosition,	//索引缓冲数据的起始偏移
				data.VertexoffsetPostion,	//顶点缓冲数据的起始偏移
				0);							//从顶点缓冲区读取每个实例数据之前添加到每个索引的值
		}
	}
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawObjectNumber(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, Handle);
}

bool FGeometry::isExitDescribeMeshRenderingData(CMesh* InKey)
{
	for (FGeometryDescData& data : DescribeMeshRenderingData)
	{
		if (data.Mesh == InKey)
		{
			return true;
		}
	}
	return false;
}

void FGeometry::BuildMeshDescData(CMesh* InMesh, const FMeshRenderingData& MeshRenderData)
{
	if (!isExitDescribeMeshRenderingData(InMesh))
	{
		DescribeMeshRenderingData.emplace_back(FGeometryDescData());
		FGeometryDescData& GeometryDescData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];
		GeometryDescData.Mesh = InMesh;
		GeometryDescData.IndexSize = MeshRenderData.IndexData.size();
		GeometryDescData.IndexoffsetPosition = MeshRenderingData.IndexData.size();
		GeometryDescData.VertexSize = MeshRenderData.VertexData.size();
		GeometryDescData.VertexoffsetPostion = MeshRenderingData.VertexData.size();

		MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshRenderData.IndexData.begin(), MeshRenderData.IndexData.end());
		MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshRenderData.VertexData.begin(), MeshRenderData.VertexData.end());

	}
}

void FGeometry::BuildMeshBuffer(const int& InIndex)
{
	//在cpu中创建资源缓冲区
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetVertexSizeInBytes(), &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), MeshRenderingData.GetVertexSizeInBytes());
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetIndexSizeInBytes(), &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), MeshRenderingData.GetIndexSizeInBytes());

	//在gpu中创建资源缓冲区
	ConstructBuffer::FConstructBuffer ConstructBuffer{};
	GPUVertexBufferPtr = ConstructBuffer.ConstructGPUDefaultBuffer(VertexBufferTempPtr, MeshRenderingData.VertexData.data(), MeshRenderingData.GetVertexSizeInBytes());
	GPUIndexBufferPtr = ConstructBuffer.ConstructGPUDefaultBuffer(IndexBufferTempPtr, MeshRenderingData.IndexData.data(), MeshRenderingData.GetIndexSizeInBytes());
	if (GPUIndexBufferPtr == nullptr || GPUVertexBufferPtr == nullptr)
	{
		Engine_Log_Error("FGeometry::BuildMeshBuffer error");
		return;
	}
}

UINT FGeometry::GetDrawObjectNumber() const
{
	return DescribeMeshRenderingData.size();
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV = {};
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	VBV.StrideInBytes = sizeof(FVertex);
	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV = {};
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
	IBV.Format = DXGI_FORMAT_R16_UINT;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();

	return IBV;
}
