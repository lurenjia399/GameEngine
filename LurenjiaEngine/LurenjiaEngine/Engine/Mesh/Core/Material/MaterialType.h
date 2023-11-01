#pragma once

#ifndef MaterialTypeH
#define MaterialTypeH

enum class EMaterialType
{
	Default = 0,				//ֻ���ò�����ɫ
	Lambert,				//������
	HalfLambert,			//�������أ�
	Phone,					//�߹⣬����Ч��						����ӷ���
	BlinnPhone,				//�߹⣬����Ч�����ð������			����ӷ���
	WrapLight,				//ģ��Ƥ��
	MinnaertLight,			//����ޣ�˿��
	BandedLight,			//��ͨ����ģ��
	GradualBanded,			//�ֲ����ģ��
	FinalBanded,			//�ں϶���Ч���Ŀ�ͨ����ģ��
	Back,					//�α���ɢ�䣬ģ����					����ӷ���
	AnisotropyKajiyaKay,	//��������
	OrenNayar,				//ģ��ֲڱ���,����ɳĮ

	Transparency = 15,		//͸��
	ShadowMap = 16,			//��������shadowmap�Ĳ���
	//NOShowMaterial = 17,	//����ʾ����

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
	LineListDisplay,		//���б�
	WireframeDisplay,		//�߿�ķ�ʽ����������
	TriangleDisplay = 4,	//�����εķ�ʽ
};

#endif