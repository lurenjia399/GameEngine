#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialTexturePath("")
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDirty(true)
	, MaterialIndex(-1)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor)
	: BaseColor(InBaseColor)
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialTexturePath("")
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDirty(true)
	, MaterialIndex(-1)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(1.0f)
	, MaterialTexturePath("")
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDirty(true)
	, MaterialIndex(-1)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialTexturePath("")
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDirty(true)
	, MaterialIndex(-1)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, string InMaterialTexturePath)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialTexturePath(InMaterialTexturePath)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDirty(true)
	, MaterialIndex(-1)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, string InMaterialTexturePath, EMaterialDisplayStatusType InMaterialDisplayStatusType)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialTexturePath(InMaterialTexturePath)
	, MaterialDisplayStatusType(InMaterialDisplayStatusType)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDirty(true)
	, MaterialIndex(-1)
{
}

void CMaterial::SetBaseColor(const XMFLOAT4& InBaseColor)
{
	BaseColor = InBaseColor;
	SetDirty(true);
}

void CMaterial::SetBaseColor(const string& InPath)
{
	MaterialTexturePath = InPath;
	SetDirty(true);
}

void CMaterial::SetMaterialType(const EMaterialType& InMaterialType)
{
	MaterialType = InMaterialType;
	SetDirty(true);
}

void CMaterial::SetRoughness(const float& InRoughness)
{
	Roughness = InRoughness;
	SetDirty(true);
}

void CMaterial::SetMaterialDisplayStatusType(EMaterialDisplayStatusType InDisplayStatusType)
{
	MaterialDisplayStatusType = InDisplayStatusType;
	SetDirty(true);
}

void CMaterial::SetMaterialTransform(XMFLOAT4X4 InTransform)
{
	MaterialTransform = InTransform;
	SetDirty(true);
}

void CMaterial::SetDirty(bool InDirty)
{
	if (bDirty != InDirty)
	{
		bDirty = InDirty;
	}
}

void CMaterial::SetMaterialIndex(int InMaterialIndex)
{
	MaterialIndex = InMaterialIndex;
}

