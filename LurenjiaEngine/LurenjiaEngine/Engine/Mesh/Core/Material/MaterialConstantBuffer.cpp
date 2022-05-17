#include "MaterialConstantBuffer.h"
#include "../../../Math/EngineMath.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()
	: BaseColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f))
	, TransformInformation(EngineMath::IdentityMatrix4x4())
{
}
