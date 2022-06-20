#pragma once

#ifndef MaterialTypeH
#define MaterialTypeH

enum class EMaterialType
{
	Default,				//ֻ���ò�����ɫ
	Lambert,				//������
	HalfLambert,			//�������أ�
	Phone,					//�߹⣬����Ч��
	BlinnPhone,				//�߹⣬����Ч�����ð������
	WrapLight,				//ģ��Ƥ��
	MinnaertLight,			//����ޣ�˿��
	BandedLight,			//��ͨ����ģ��
	GradualBanded,			//������ģ��
	FinalBanded,			//�ں϶���Ч���Ŀ�ͨ����ģ��
	Back,					//�α���ɢ�䣬ģ����
	AnisotropyKajiyaKay,	//��������
	OrenNayar,				//ģ��ֲڱ���,����ɳĮ

	Normal = 97,			//�ֲ�����
	WorldNormal = 98,		//���編��
	BaseColor = 99,			//���ʵ���ɫ
	Fresnel = 100,			//������Ч��
	Max
};


enum class EMaterialDisplayStatusType
{
	Undefined,				//Ĭ����ʾ
	PointDisplay,			//��ķ�ʽ
	WireframeDisplay,		//�߿�ķ�ʽ
	TriangleDisplay = 4,	//�����εķ�ʽ
};

#endif