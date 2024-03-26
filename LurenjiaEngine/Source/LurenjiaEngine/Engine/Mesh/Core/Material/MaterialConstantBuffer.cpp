#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"


CMaterialConstantBuffer::CMaterialConstantBuffer()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, MaterialType(0)
	, Roughness(1.0f)
	, TextureMapIndex(-1)
	, NormalMapIndex(-1)
	, SpecularMapIndex(-1)
	, SpecularColor(XMFLOAT3(0.5f, 0.5f, 0.5f))
	, FresnelF0(XMFLOAT3(0.f, 0.f, 0.f))
	, xx(0.f)
	, TransformInformation(EngineMath::IdentityMatrix4x4())
{
}
