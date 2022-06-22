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

	void SetBaseColor(const XMFLOAT4& InBaseColor);				//���ò��ʵĻ�����ɫ
	void SetMaterialTextureMapKey(const string& InPath);		//���ò�����ͼ��·��
	void SetMaterialTextureMapIndex(int InMaterialIndex);
	void SetMaterialNormalMapKey(const string& InPath);
	void SetMaterialNormalMapIndex(int InMaterialIndex);
	void SetMaterialType(const EMaterialType& InMaterialType);
	void SetRoughness(const float& InRoughness);
	void SetMaterialDisplayStatusType(EMaterialDisplayStatusType InDisplayStatusType);
	void SetMaterialTransform(XMFLOAT4X4 InTransform);
	void SetDirty(bool InDirty);
	

	FORCEINLINE XMFLOAT4					GetBaseColor() const					{ return BaseColor; }
	FORCEINLINE EMaterialType				GetMaterialType() const					{ return MaterialType; }
	FORCEINLINE float						GetRoughness() const					{ return Roughness; }
	FORCEINLINE string						GetMaterialTextureMapKey() const		{ return MaterialTextureMapKey; }
	FORCEINLINE int							GetMaterialTextureMapIndex() const		{ return MaterialTextureMapIndex; }
	FORCEINLINE string						GetMaterialNormalMapKey() const			{ return MaterialNormalMapKey; }
	FORCEINLINE int							GetMaterialNormalMapIndex() const		{ return MaterialNormalMapIndex; }
	FORCEINLINE EMaterialDisplayStatusType	GetMaterialDisplayStatusType() const	{ return MaterialDisplayStatusType; }
	FORCEINLINE XMFLOAT4X4					GetMaterialTransform() const			{ return MaterialTransform; }
	FORCEINLINE bool						isDirty() const							{ return bDirty; }
	
private:
	XMFLOAT4 BaseColor;						//���ʵĻ�����ɫ
	EMaterialType MaterialType;				//���ʵ�����
	float Roughness;						//���ʵĴֲڶ�
	string MaterialTextureMapKey;			//����ʹ�õ�������ͼKey
	int MaterialTextureMapIndex;			//����ʹ�õ�������ͼIndex
	string MaterialNormalMapKey;			//����ʹ�õķ�����ͼKey
	int MaterialNormalMapIndex;				//����ʹ�õķ�����ͼIndex
	EMaterialDisplayStatusType MaterialDisplayStatusType;//���ʵ���ʾ��ʽ
	XMFLOAT4X4 MaterialTransform;			//���ʵı任��Ϣ��Ӧ����ģ�͵�uv����
	bool bDirty;							//��־��ǰ�����Ƿ�̬����
	
};

#endif // !Material
