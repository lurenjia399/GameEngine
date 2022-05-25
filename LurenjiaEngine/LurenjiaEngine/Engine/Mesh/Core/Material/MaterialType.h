#pragma once

#ifndef MaterialTypeH
#define MaterialTypeH

enum class EMaterialType
{
	Default,
	Lambert,//兰伯特
	HalfLambert,//半兰伯特，
	Phone,//高光，金属效果
	BlinnPhone,//高光，金属效果，用半程向量
	WrapLight,//模拟皮肤
	MinnaertLight,//天鹅绒，丝袜
	BandedLight,//卡通光照模型
	GradualBanded,//封层光照模型
	FinalBanded,//融合多种效果的卡通光照模型
	Back,//次表面散射，模拟玉
	AnisotropyKajiyaKay,//各向异性
	Fresnel = 100,//菲涅尔效果
	Max
};


#endif