#pragma once

#ifndef MaterialTypeH
#define MaterialTypeH

enum class EMaterialType
{
	Default,
	Lambert,//������
	HalfLambert,//�������أ�
	Phone,//�߹⣬����Ч��
	BlinnPhone,//�߹⣬����Ч�����ð������
	WrapLight,//ģ��Ƥ��
	MinnaertLight,//����ޣ�˿��
	BandedLight,//��ͨ����ģ��
	GradualBanded,//������ģ��
	FinalBanded,//�ں϶���Ч���Ŀ�ͨ����ģ��
	Back,//�α���ɢ�䣬ģ����
	AnisotropyKajiyaKay,//��������
	Fresnel = 100,//������Ч��
	Max
};


#endif