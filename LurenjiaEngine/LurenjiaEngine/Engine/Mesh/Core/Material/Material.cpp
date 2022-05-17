#include "Material.h"

CMaterial::CMaterial()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
{
}

CMaterial::CMaterial(XMFLOAT4 InBaseColor)
	: BaseColor(InBaseColor)
{
}

void CMaterial::SetBaseColor(const XMFLOAT4& InBaseColor)
{
	BaseColor = InBaseColor;
}

