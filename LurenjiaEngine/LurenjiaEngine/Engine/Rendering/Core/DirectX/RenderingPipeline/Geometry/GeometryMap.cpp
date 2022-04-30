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
	//+1���������
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

			//object��������������ģ�ͱ任����
			FObjectTransformation ObjectTransformation;
			XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
			ObjectConstantBufferView.Update(i, &ObjectTransformation);
		}
	}
	//viewport��������������������任�����͸��ͶӰ����
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ViewProjection = XMMatrixTranspose(ViewMatrix) * ProjectMatrix;//�м���Ҫת�ã��������еľ����޷������еľ���
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, ViewProjection);//ע��constBuffer�ж�ȡ�����ǰ����ж�ȡ��
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
			//�������б��� ��Ӷ��㻺������ ����
			GetGraphicsCommandList()->IASetVertexBuffers(0, 1, &VBV);
			//�������б��� ��������������� ����
			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
			//�������б��� ���ͼԪ���� ����
			GetGraphicsCommandList()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//ƫ��
			Handle.Offset(i, HandleSize);
			//�������б��� ��ӽ�����������ӵ���ǩ���� ����
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, Handle);
			// Draw Call !!!
			GetGraphicsCommandList()->DrawIndexedInstanced(
				data.IndexSize,				//���Ƶ�ʵ���������������
				1,							//���Ƶ�ʵ������
				data.IndexoffsetPosition,	//�����������ݵ���ʼƫ��
				data.VertexoffsetPostion,	//���㻺�����ݵ���ʼƫ��
				0);							//�Ӷ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ
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
	//��cpu�д�����Դ������
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetVertexSizeInBytes(), &CPUVertexBufferPtr));
	memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), MeshRenderingData.GetVertexSizeInBytes());
	ANALYSIS_HRESULT(D3DCreateBlob(MeshRenderingData.GetIndexSizeInBytes(), &CPUIndexBufferPtr));
	memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), MeshRenderingData.GetIndexSizeInBytes());

	//��gpu�д�����Դ������
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
