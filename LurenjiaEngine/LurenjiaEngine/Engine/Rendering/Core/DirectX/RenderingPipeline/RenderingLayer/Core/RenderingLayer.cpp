#include "RenderingLayer.h"
#include "../RenderLayerManage.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"

FRenderingLayer::FRenderingLayer()
	: RenderingLayerPriority(0)
	, GeometryMap(nullptr)
	, DirectXPiepelineState(nullptr)
{
}

void FRenderingLayer::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	GeometryMap = InGeometryMap;
	DirectXPiepelineState = InDirectXPiepelineState;
}

int FRenderingLayer::GetRenderLayerType() const
{
	return 0;
}

void FRenderingLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderingLayers.emplace_back(shared_from_this());
}

UINT FRenderingLayer::GetPriority()
{
	return RenderingLayerPriority;
}

std::vector<FGeometryDescData>* FRenderingLayer::GetGeometryDescData()
{
	return &GeometryDescDatas;
}

void FRenderingLayer::UpdateObjectConstantBuffer()
{
	for (auto& data : GeometryDescDatas)
	{
		const XMFLOAT3& Position = data.MeshComponet->GetPosition();
		const XMFLOAT3& Scale = data.MeshComponet->GetScale();
		const XMFLOAT3& RightVector = data.MeshComponet->GetRight();
		const XMFLOAT3& UpVector = data.MeshComponet->GetUp();
		const XMFLOAT3& ForwardVector = data.MeshComponet->GetForward();

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
		XMMATRIX MatrixTextureTransform = XMLoadFloat4x4(&data.TextureTransform);

		//����shader�е�����任 ����������
		FObjectTransformation ObjectTransformation;
		XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
		XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(MatrixTextureTransform));
		ObjectTransformation.MaterialIndex = data.MeshComponet->GetMaterials()->at(0)->GetMaterialTextureMapIndex();
		GeometryMap->MeshConstantBufferView.Update(data.MeshObjectOffset, &ObjectTransformation);
	}
}

void FRenderingLayer::PreDraw(float DeltaTime)
{
}

void FRenderingLayer::Draw(float DeltaTime)
{
	UINT HandleSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (auto& data : GeometryDescDatas)
	{
		//��ȡgeometry�Ķ��㻺��������
		D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometrys[data.GeometryKey].GetVertexBufferView();
		//��ȡgeometry����������������
		D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometrys[data.GeometryKey].GetIndexBufferView();

		//��ȡָ�����ѵ�handleָ��
		CD3DX12_GPU_DESCRIPTOR_HANDLE meshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetDescriptorHeap()->GetHeap()->GetGPUDescriptorHandleForHeapStart());

		//�������б��� ��Ӷ��㻺��������(����geometry�Ķ�����Ϣ)
		GetGraphicsCommandList()->IASetVertexBuffers(0, 1, &VBV);
		//�������б��� �����������������(����geometry��������Ϣ)
		GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
		//�������б��� ���ͼԪ���� ����(����geometry�����˷�ʽ)
		EMaterialDisplayStatusType TopologyType = (*data.MeshComponet->GetMaterials())[0]->GetMaterialDisplayStatusType();
		GetGraphicsCommandList()->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)TopologyType);
		//ָ�����ѵ�ָ��ƫ��
		meshHandle.Offset(data.MeshObjectOffset, HandleSize);
		//�������б��� ��ӽ�����������ӵ���ǩ���� ����
		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, meshHandle);

		// Draw Call !!!
		GetGraphicsCommandList()->DrawIndexedInstanced(
			data.IndexSize,				//���Ƶ�ʵ���������������
			1,							//���Ƶ�ʵ������
			data.IndexoffsetPosition,	//�����������ݵ���ʼƫ��
			data.VertexoffsetPostion,	//���㻺�����ݵ���ʼƫ��
			0);							//�Ӷ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ
	}

}

void FRenderingLayer::PostDraw(float DeltaTime)
{
}

void FRenderingLayer::BuildPSO()
{
}
