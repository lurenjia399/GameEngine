#include "FogConstantBuffer.h"

FFogConstantBuffer::FFogConstantBuffer()
	: FogColor(XMFLOAT3(1.0f,1.0f,1.0f))
	, FogStart(10.f)
	, FogRange(100.f)
	, FogHeight(9000.f)
	, FogTransparentCoefficient(0.f)
{
}
