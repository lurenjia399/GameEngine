#include "ObjectTransformation.h"
#include "../../Math/EngineMath.h"

FObjectTransformation::FObjectTransformation()
	: World(EngineMath::IdentityMatrix4x4())
{}

