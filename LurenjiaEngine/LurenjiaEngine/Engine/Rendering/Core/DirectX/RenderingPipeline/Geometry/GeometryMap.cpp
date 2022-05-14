#include "GeometryMap.h"
#include "../../../../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"

FGeometryMap::FGeometryMap()
{
	Geometrys.emplace(0, FGeometry());
}

void FGeometryMap::BuildMeshDescData(AMesh* InMesh, const FMeshRenderingData& InRenderingData)
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
	//+1 摄像机
	DescriptorHeap.BuildDescriptorHeap(GetDrawMeshObjectCount() + GetDrawMaterialObjectCount() + GetDrawLightObjectCount() + 1);
}

void FGeometryMap::BuildMeshConstantBufferView()
{
	MeshConstantBufferView.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectCount());
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	MeshConstantBufferView.BuildConstantBuffer(Handle, GetDrawMeshObjectCount(), 0);
}

void FGeometryMap::BuildMaterialConstantBufferView()
{
	MeshConstantBufferView.CreateConstant(sizeof(CMaterialConstantBuffer), GetDrawMaterialObjectCount());
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	MeshConstantBufferView.BuildConstantBuffer(Handle, GetDrawMaterialObjectCount(), GetDrawMeshObjectCount());
}

void FGeometryMap::BuildLightConstantBufferView()
{
	LightConstantBufferView.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectCount());
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	LightConstantBufferView.BuildConstantBuffer(Handle, GetDrawLightObjectCount(), GetDrawMeshObjectCount() + GetDrawMaterialObjectCount());
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	ViewportConstantBufferView.CreateConstant(sizeof(FViewportTransformation), 1);
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart());
	ViewportConstantBufferView.BuildConstantBuffer(Handle, 1, GetDrawMeshObjectCount() + GetDrawMaterialObjectCount() + GetDrawLightObjectCount());
}

UINT FGeometryMap::GetDrawMeshObjectCount()
{
	return Geometrys[0].GetDrawMeshObjectCount();
}

UINT FGeometryMap::GetDrawMaterialObjectCount()
{
	UINT res = 0;
	for (pair<int, FGeometry> Geometry : Geometrys)
	{
		res += Geometry.second.GetDrawMaterialObjectCount();
	}
	return res;
}

UINT FGeometryMap::GetDrawLightObjectCount()
{
	return 1;
}

void FGeometryMap::UpdateConstantView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (pair<const int, FGeometry> temp : Geometrys)
	{
		for (UINT i = 0; i < temp.second.DescribeMeshRenderingData.size(); ++i)
		{
			FGeometryDescData& data = temp.second.DescribeMeshRenderingData[i];
			const XMFLOAT3& Position = data.Mesh->GetPosition();
			const XMFLOAT3& Scale = data.Mesh->GetScale();
			const XMFLOAT3& RightVector = data.Mesh->GetRight();
			const XMFLOAT3& UpVector = data.Mesh->GetUp();
			const XMFLOAT3& ForwardVector = data.Mesh->GetForward();

			XMMATRIX scaleMatrix =
			{
				Scale.y,	0.0f,		0.0f,		0.0f,
				0.0f,		Scale.z,	0.0f,		0.0f,
				0.0f,		0.0f,		Scale.x,	0.0f,
				0.0f,		0.0f,		0.0f,		1.0f
			};

			XMMATRIX rotateMatrix =
			{
				RightVector.x,	UpVector.x, ForwardVector.x,	0.f,
				RightVector.y,	UpVector.y, ForwardVector.y,	0.f,
				RightVector.z,	UpVector.z, ForwardVector.z,	0.f,
				0,				0,			0,					1.f
			};
			XMMATRIX TranslateMatrix =
			{
				1.f, 0.f, 0.f, Position.x,
				0.f, 1.f, 0.f, Position.y,
				0.f, 0.f, 1.f, Position.z,
				0.f, 0.f, 0.f, 1.f
			};
			XMStoreFloat4x4(&data.WorldMatrix, XMMatrixTranspose(TranslateMatrix * rotateMatrix * scaleMatrix));
			XMMATRIX MatrixWorld = XMLoadFloat4x4(&data.WorldMatrix);

			//object常量缓冲区传入模型变换矩阵
			FObjectTransformation ObjectTransformation;
			XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
			MeshConstantBufferView.Update(i, &ObjectTransformation);
		}
	}

	FLightConstantBuffer lightTransformation;
	//更新light的常量缓冲区数据
	LightConstantBufferView.Update(0, &lightTransformation);


	//viewport常量缓冲区传入摄像机变换矩阵和透视投影矩阵
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//切记需要转置，，，主列的矩阵无法乘主行的矩阵
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//注意shader读取constBuffer中数据是按照列读取的
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

void FGeometryMap::DrawViewport(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawMeshObjectCount() + GetDrawMaterialObjectCount() + GetDrawLightObjectCount(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, Handle);
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
			CD3DX12_GPU_DESCRIPTOR_HANDLE meshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
			CD3DX12_GPU_DESCRIPTOR_HANDLE materialHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
			
			FGeometryDescData& data = tem.second.DescribeMeshRenderingData[i];
			//向命令列表中 添加顶点缓冲数据 命令
			GetGraphicsCommandList()->IASetVertexBuffers(0, 1, &VBV);
			//向命令列表中 添加索引缓冲数据 命令
			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
			//向命令列表中 添加图元拓扑 命令
			GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//模型偏移
			meshHandle.Offset(i, HandleSize);
			//向命令列表中 添加将描述符表添加到根签名中 命令
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, meshHandle);

			//材质偏移
			materialHandle.Offset(GetDrawMeshObjectCount() + i, HandleSize);
			//向命令列表中 添加将描述符表添加到根签名中 命令
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, materialHandle);

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

void FGeometryMap::DrawLight(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE Handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(DescriptorHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());
	Handle.Offset(GetDrawMeshObjectCount() + GetDrawMaterialObjectCount(), HandleSize);
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, Handle);
}

bool FGeometry::isExitDescribeMeshRenderingData(AMesh* InKey)
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

void FGeometry::BuildMeshDescData(AMesh* InMesh, const FMeshRenderingData& MeshRenderData)
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

UINT FGeometry::GetDrawMeshObjectCount() const
{
	return DescribeMeshRenderingData.size();
}

UINT FGeometry::GetDrawMaterialObjectCount() const
{
	UINT res = 0;
	for (FGeometryDescData GeometryDesc : DescribeMeshRenderingData)
	{
		res += GeometryDesc.Mesh->GetMaterialsCount();
	}
	return res;
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
