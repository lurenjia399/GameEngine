#include "ObjectConstantBuffer.h"
#include "../../../Math/EngineMath.h"

FMeshConstantBuffer::FMeshConstantBuffer()
	: World(EngineMath::IdentityMatrix4x4())
	, TextureTransformation(EngineMath::IdentityMatrix4x4())
	, MaterialIndex(-1)
{}

