#pragma once

#ifndef Material
#define Material

#include "../../../Core/CoreObject/CoreMinimalObject.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();
	CMaterial(XMFLOAT4 InBaseColor);

	void SetBaseColor(const XMFLOAT4& InBaseColor);
	FORCEINLINE XMFLOAT4 GetBaseColor() const { return BaseColor; }
private:
	XMFLOAT4 BaseColor;
};

#endif // !Material
