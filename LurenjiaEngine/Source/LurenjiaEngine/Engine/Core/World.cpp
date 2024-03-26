#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	camera = LurenjiaEngine::CreateObject<ACamera>(this, "camera");
	camera->SetPosition(XMFLOAT3(-24.f, 0.f, 2.f));
	fog = nullptr;
}

std::shared_ptr<AFog> CWorld::GetFog() const
{
	return fog;
}

bool CWorld::LineTraceSingleByChannel(FHitResult& OutHitResult, const XMFLOAT3& Start, const XMFLOAT3& End) const
{
	return false;
}


