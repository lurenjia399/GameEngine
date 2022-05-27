#include "LightConstantBuffer.h"

FLightConstantBuffer::FLightConstantBuffer()
	: LightIntensity(XMFLOAT3(1.0f, 1.0f, 1.0f))
	, XX(0)
	, LightDirection(XMFLOAT3(0.f, -1.f, 0.f))
	, XX2(0)
{
}
