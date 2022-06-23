#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"


CMaterialConstantBuffer::CMaterialConstantBuffer()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, MaterialType(0)
	, Roughness(1.0f)
	, TextureMapIndex(-1)
	, NormalMapIndex(-1)
	, SpecularIndex(-1)
	, TransformInformation(EngineMath::IdentityMatrix4x4())
{
}
