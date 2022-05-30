#include "Material.h"

CMaterial::CMaterial()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor)
	: BaseColor(InBaseColor)
	, MaterialType(EMaterialType::Default)
	, Roughness(1.0f)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(1.0f)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialDisplayStatusType(EMaterialDisplayStatusType::TriangleDisplay)
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness, EMaterialDisplayStatusType InMaterialDisplayStatusType)
	: BaseColor(InBaseColor)
	, MaterialType(InMaterialType)
	, Roughness(InRoughness)
	, MaterialDisplayStatusType(InMaterialDisplayStatusType)
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

