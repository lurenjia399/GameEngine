#pragma once
#include "../../../../../Core/Engine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Mesh/Core/Mesh.h"

struct FGeometryDescData : public IDirectXDeviceInterface_struct
{
public:
	FGeometryDescData();

public:
	UINT GetVertexSizeInBytes() const;						//��ȡ�������ݴ�С
	UINT GetIndexSizeInBytes() const;						//��ȡ�������ݴ�С


	CMeshComponent* MeshComponet;							//�����ģ�����
	size_t MeshHash;										//�����ģ�͵Ĺ�ϣֵ
public:

	UINT IndexSize;											//����������
	UINT VertexSize;										//���������
	UINT IndexoffsetPosition;								//����λ��ƫ��
	UINT VertexoffsetPostion;								//����λ��ƫ��
	UINT IndexTypeSize;										//��������size
	UINT VertexTypeSize;									//��������size


	XMFLOAT4X4 WorldMatrix;									//m�任����
	XMFLOAT4X4 TextureTransform;							//��ͼ�ľ���
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	//��Ҫ���µ���Դ
};