#pragma once

#ifndef MaterialTypeH
#define MaterialTypeH

enum class EMaterialType
{
	Default,
	Lambert,
	HalfLambert,
	Phone,
	BlinnPhone,
	WrapLight,
	MinnaertLight,
	BandedLight,
	GradualBanded,
	FinalBanded,
	Back,
	Fresnel = 100,//������Ч��
	Max
};


#endif