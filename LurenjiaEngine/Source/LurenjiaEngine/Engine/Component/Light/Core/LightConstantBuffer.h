#pragma once

#ifndef LightConstantBuffer_H
#define LightConstantBuffer_H
#include "../../../../Engine/Core/Engine.h"

struct FLight
{
	XMFLOAT3	LightIntensity;			//��Դǿ��
	float		StartAttenuation;		//���Դ��ʼ˥�����루����Դ����λ�õľ��룩

	XMFLOAT3	LightDirection;			//��Դ����
	float		EndAttenuation;			//���Դ�Ľ���˥��������Դ����λ�õľ��룩

	XMFLOAT3	Position;               //�ƹ�λ��
	int			LightType;              //�ƹ�����

	float		ConicalInnerCorner;     //�۹��׶���ڽǣ�������
	float		ConicalOuterCorner;     //�۹��׶����ǣ�������
	int			XX;                     //ռλ
	int			XX2;                    //ռλ

	XMFLOAT4X4	ViewProjectionMatrix;	//�ƹ���ӿ�ͶӰ����������shadowmap��
};



struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLight[16];
};

#endif // !LightConstantBuffer
