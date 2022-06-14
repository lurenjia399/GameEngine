#include "Material.h"
#include "../../../Math/EngineMath.h"

CMaterial::CMaterial()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor)
	: BaseColor(InBaseColor)
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(1.0f)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, EMaterialDisplayStatusType InMaterialDisplayStatusType)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialDisplayStatusType(InMaterialDisplayStatusType)
	, MaterialTransform(EngineMath::IdentityMatrix4x4())
{
}

void CMaterial::SetBaseColor(const XMFLOAT4& InBaseColor)
{
	BaseColor = InBaseColor;
}

void CMaterial::SetMaterialType(const EMaterialType& InMaterialType)
{
	MaterialType = InMaterialType;
}

void CMaterial::SetRoughness(const float& InRoughness)
{
	Roughness = InRoughness;
}

void CMaterial::SetMaterialDisplayStatusType(EMaterialDisplayStatusType InDisplayStatusType)
{
	MaterialDisplayStatusType = InDisplayStatusType;
}

void CMaterial::SetMaterialTransform(XMFLOAT4X4 InTransform)
{
	MaterialTransform = InTransform;
}

