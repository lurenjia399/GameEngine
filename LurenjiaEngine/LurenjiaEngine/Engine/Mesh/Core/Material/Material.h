#pragma once

#ifndef Material
#define Material

#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();
	CMaterial(XMFLOAT4 InBaseColor);
	CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType);
	CMaterial(XMFLOAT4 InBaseColor, EMaterialType InMaterialType, float InRoughness);

	void SetBaseColor(const XMFLOAT4& InBaseColor);
	void SetMaterialType(const EMaterialType& InMaterialType);
	void SetRoughness(const float& InRoughness);

	FORCEINLINE XMFLOAT4 GetBaseColor() const { return BaseColor; }
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	FORCEINLINE float GetRoughness()const { return Roughness; }
private:
	XMFLOAT4 BaseColor;
	EMaterialType MaterialType;
	float Roughness;
};

#endif // !Material