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

	void SetBaseColor(const XMFLOAT4& InBaseColor);				//设置材质的基础颜色
	void SetSpecularColor(const XMFLOAT3& InSpecularColor);		//设置材质的高光颜色
	void SetMaterialTextureMapKey(const string& InPath);		//设置材质贴图的路径
	void SetMaterialTextureMapIndex(int InMaterialIndex);
	void SetMaterialNormalMapKey(const string& InPath);
	void SetMaterialNormalMapIndex(int InMaterialIndex);
	void SetMaterialSpecularMapKey(const string& InPath);
	void SetMaterialSpecularMapIndex(int InMaterialIndex);
	void SetMaterialType(const EMaterialType& InMaterialType);
	void SetRoughness(const float& InRoughness);
	void SetMaterialDisplayStatusType(EMaterialDisplayStatusType InDisplayStatusType);
	void SetMaterialTransform(XMFLOAT4X4 InTransform);
	void SetDirty(bool InDirty);
	

	FORCEINLINE XMFLOAT4					GetBaseColor() const					{ return BaseColor; }
	FORCEINLINE XMFLOAT3					GetSpecularColor() const				{ return SpecularColor; }
	FORCEINLINE EMaterialType				GetMaterialType() const					{ return MaterialType; }
	FORCEINLINE float						GetRoughness() const					{ return Roughness; }
	FORCEINLINE string						GetMaterialTextureMapKey() const		{ return MaterialTextureMapKey; }
	FORCEINLINE int							GetMaterialTextureMapIndex() const		{ return MaterialTextureMapIndex; }
	FORCEINLINE string						GetMaterialNormalMapKey() const			{ return MaterialNormalMapKey; }
	FORCEINLINE int							GetMaterialNormalMapIndex() const		{ return MaterialNormalMapIndex; }
	FORCEINLINE string						GetMaterialSpecularMapKey() const		{ return MaterialSpecularMapKey; }
	FORCEINLINE int							GetMaterialSpecularMapIndex() const		{ return MaterialSpecularMapIndex; }
	FORCEINLINE EMaterialDisplayStatusType	GetMaterialDisplayStatusType() const	{ return MaterialDisplayStatusType; }
	FORCEINLINE XMFLOAT4X4					GetMaterialTransform() const			{ return MaterialTransform; }
	FORCEINLINE bool						isDirty() const							{ return bDirty; }
	
private:
	XMFLOAT4 BaseColor;						//材质的基本颜色
	XMFLOAT3 SpecularColor;					//材质的高光颜色
	EMaterialType MaterialType;				//材质的类型
	float Roughness;						//材质的粗糙度
	string MaterialTextureMapKey;			//材质使用的纹理贴图Key
	int MaterialTextureMapIndex;			//材质使用的纹理贴图Index
	string MaterialNormalMapKey;			//材质使用的法线贴图Key
	int MaterialNormalMapIndex;				//材质使用的法线贴图Index
	string MaterialSpecularMapKey;			//材质使用的高光贴图Key
	int MaterialSpecularMapIndex;			//材质使用的高光贴图Index
	EMaterialDisplayStatusType MaterialDisplayStatusType;//材质的显示方式
	XMFLOAT4X4 MaterialTransform;			//材质的变换信息，应用于模型的uv坐标
	bool bDirty;							//标志当前材质是否动态更改
};

#endif // !Material
