#include "ViewportTransformation.h"
#include "../../Math/EngineMath.h"

FViewportTransformation::FViewportTransformation()
	: cameraPosition(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
	, ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}
