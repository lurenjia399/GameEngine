#include "ViewportConstantBuffer.h"
#include "../../Math/EngineMath.h"

FViewportConstantBuffer::FViewportConstantBuffer()
	: cameraPosition(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
	, ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}
