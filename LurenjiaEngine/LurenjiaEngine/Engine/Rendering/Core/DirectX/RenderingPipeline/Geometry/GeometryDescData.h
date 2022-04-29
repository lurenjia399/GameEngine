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


	CMesh* Mesh;											//�����ģ��

public:
	DXGI_FORMAT IndexFormat;								//������Դ��ʽ

	UINT IndexSize;											//����������
	UINT VertexSize;										//���������
	UINT IndexoffsetPosition;								//����λ��ƫ��
	UINT VertexoffsetPostion;								//����λ��ƫ��
	UINT IndexTypeSize;										//��������size
	UINT VertexTypeSize;									//��������size


	XMFLOAT4X4 WorldMatrix;									//mvp�任����
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	//��Ҫ���µ���Դ
};