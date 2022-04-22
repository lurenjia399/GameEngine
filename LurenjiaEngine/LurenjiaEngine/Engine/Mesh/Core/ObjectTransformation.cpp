#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
	: World(FObjectTransformation::IdentityMatrix4x4())
{}

XMFLOAT4X4 FObjectTransformation::IdentityMatrix4x4()
{
	return XMFLOAT4X4(
		1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1.0f);
}