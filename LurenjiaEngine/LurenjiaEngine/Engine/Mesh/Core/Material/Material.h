#pragma once

#ifndef Material_H
#define Material_H

#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();
	CMaterial(XMFLOAT4 InBaseColor);
	CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType);
	CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness);
	CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, string InMaterialTexturePath);
	CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, string InMaterialTexturePath, EMaterialDisplayStatusType InMaterialDisplayStatusType);

	void SetBaseColor(const XMFLOAT4& InBaseColor);	//设置材质的基础颜色
	void SetBaseColor(const string& InPath);		//设置材质贴图的路径
	void SetMaterialType(const EMaterialType& InMaterialType);
	void SetRoughness(const float& InRoughness);
	void SetMaterialDisplayStatusType(EMaterialDisplayStatusType InDisplayStatusType);
	void SetMaterialTransform(XMFLOAT4X4 InTransform);
	void SetDirty(bool InDirty);
	void SetMaterialIndex(int InMaterialIndex);

	FORCEINLINE XMFLOAT4					GetBaseColor() const					{ return BaseColor; }
	FORCEINLINE EMaterialType				GetMaterialType() const					{ return MaterialType; }
	FORCEINLINE float						GetRoughness() const					{ return Roughness; }
	FORCEINLINE string						GetMaterialTexturePath() const			{ return MaterialTexturePath; }
	FORCEINLINE EMaterialDisplayStatusType	GetMaterialDisplayStatusType() const	{ return MaterialDisplayStatusType; }
	FORCEINLINE XMFLOAT4X4					GetMaterialTransform() const			{ return MaterialTransform; }
	FORCEINLINE bool						isDirty() const							{ return bDirty; }
	FORCEINLINE int							GetMaterialIndex() const				{ return MaterialIndex; }
private:
	XMFLOAT4 BaseColor;
	EMaterialType MaterialType;
	float Roughness;
	string MaterialTexturePath;
	EMaterialDisplayStatusType MaterialDisplayStatusType;
	XMFLOAT4X4 MaterialTransform;
	bool bDirty;	//标志当前材质是否动态更改
	int MaterialIndex;
};

#endif // !Material
