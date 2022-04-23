#include "ViewportTransformation.h"
#include "../../Math/EngineMath.h"

FViewportTransformation::FViewportTransformation()
	:ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}
