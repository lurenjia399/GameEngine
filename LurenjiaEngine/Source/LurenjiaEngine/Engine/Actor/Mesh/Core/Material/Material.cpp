#include "Material.h"
#include "../../../../Math/EngineMath.h"

CMaterial::CMaterial()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialTextureMapKey("")
	, MaterialTextureMapIndex(-1)
	, MaterialNormalMapKey("")
	, MaterialNormalMapIndex(-1)
	, MaterialSpecularMapKey("")
	, MaterialSpecularMapIndex(-1)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDynamicReflection(false)
	, bDirty(true)
	
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor)
	: BaseColor(InBaseColor)
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialTextureMapKey("")
	, MaterialTextureMapIndex(-1)
	, MaterialNormalMapKey("")
	, MaterialNormalMapIndex(-1)
	, MaterialSpecularMapKey("")
	, MaterialSpecularMapIndex(-1)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDynamicReflection(false)
	, bDirty(true)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType)
	: BaseColor(InBaseColor)
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, MaterialType(InMaterialType)
	, Roughness(1.0f)
	, MaterialTextureMapKey("")
	, MaterialTextureMapIndex(-1)
	, MaterialNormalMapKey("")
	, MaterialNormalMapIndex(-1)
	, MaterialSpecularMapKey("")
	, MaterialSpecularMapIndex(-1)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDynamicReflection(false)
	, bDirty(true)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness)
	: BaseColor(InBaseColor)
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialTextureMapKey("")
	, MaterialTextureMapIndex(-1)
	, MaterialNormalMapKey("")
	, MaterialNormalMapIndex(-1)
	, MaterialSpecularMapKey("")
	, MaterialSpecularMapIndex(-1)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDynamicReflection(false)
	, bDirty(true)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, string InMaterialTexturePath)
	: BaseColor(InBaseColor)
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialTextureMapKey(InMaterialTexturePath)
	, MaterialTextureMapIndex(-1)
	, MaterialNormalMapKey("")
	, MaterialNormalMapIndex(-1)
	, MaterialSpecularMapKey("")
	, MaterialSpecularMapIndex(-1)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDynamicReflection(false)
	, bDirty(true)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, string InMaterialTexturePath, EMaterialDisplayStatusType InMaterialDisplayStatusType)
	: BaseColor(InBaseColor)
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialTextureMapKey(InMaterialTexturePath)
	, MaterialTextureMapIndex(-1)
	, MaterialNormalMapKey("")
	, MaterialNormalMapIndex(-1)
	, MaterialSpecularMapKey("")
	, MaterialSpecularMapIndex(-1)
	, MaterialDisplayStatusType(InMaterialDisplayStatusType)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
	, bDynamicReflection(false)
	, bDirty(true)
{
}

void CMaterial::SetBaseColor(const XMFLOAT4& InBaseColor)
{
	BaseColor = InBaseColor;
	SetDirty(true);
}

void CMaterial::SetSpecularColor(const XMFLOAT3& InSpecularColor)
{
	SpecularColor = InSpecularColor;
	SetDirty(true);
}

void CMaterial::SetMaterialTextureMapKey(const string& InPath)
{
	MaterialTextureMapKey = InPath;
	SetDirty(true);
}

void CMaterial::SetMaterialSpecularMapIndex(int InMaterialIndex)
{
	MaterialSpecularMapIndex = InMaterialIndex;
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

void CMaterial::SetMaterialFresnelF0(XMFLOAT3 InFresnelF0)
{
	FresnelF0 = InFresnelF0;
	SetDirty(true);
}

void CMaterial::SetDynamicReflection(bool InDynamicReflection)
{
	if (MaterialType == EMaterialType::Back || MaterialType == EMaterialType::Phone || MaterialType == EMaterialType::BlinnPhone)
	{
		bDynamicReflection = InDynamicReflection;
	}
	else
	{
		bDynamicReflection = false;
	}
	SetDirty(true);
}

void CMaterial::SetDirty(bool InDirty)
{
	if (bDirty != InDirty)
	{
		bDirty = InDirty;
	}
}

void CMaterial::SetMaterialTextureMapIndex(int InMaterialIndex)
{
	MaterialTextureMapIndex = InMaterialIndex;
}

void CMaterial::SetMaterialNormalMapKey(const string& InPath)
{
	MaterialNormalMapKey = InPath;
	SetDirty(true);
}

void CMaterial::SetMaterialNormalMapIndex(int InMaterialIndex)
{
	MaterialNormalMapIndex = InMaterialIndex;
}

void CMaterial::SetMaterialSpecularMapKey(const string& InPath)
{
	MaterialSpecularMapKey = InPath;
	SetDirty(true);
}

