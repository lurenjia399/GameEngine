#pragma once

#ifndef MaterialTypeH
#define MaterialTypeH

enum class EMaterialType
{
	Default = 0,				//只是用材质颜色
	Lambert,				//兰伯特
	HalfLambert,			//半兰伯特，
	Phone,					//高光，金属效果
	BlinnPhone,				//高光，金属效果，用半程向量
	WrapLight,				//模拟皮肤
	MinnaertLight,			//天鹅绒，丝袜
	BandedLight,			//卡通光照模型
	GradualBanded,			//封层光照模型
	FinalBanded,			//融合多种效果的卡通光照模型
	Back,					//次表面散射，模拟玉
	AnisotropyKajiyaKay,	//各向异性
	OrenNayar,				//模拟粗糙表面,比如沙漠

	Transparency = 15,		//透明

	Normal = 97,			//局部法线
	WorldNormal = 98,		//世界法线
	BaseColor = 99,			//材质的颜色
	Fresnel = 100,			//菲涅尔效果
	Max
};


enum class EMaterialDisplayStatusType
{
	Undefined,				//默认显示
	PointDisplay,			//点的方式
	LineListDisplay,		//行列表
	WireframeDisplay,		//线框的方式，线条条带
	TriangleDisplay = 4,	//三角形的方式
};

#endif