#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	camera = LurenjiaEngine::CreateObject<ACamera>("camera");
	camera->SetPosition(XMFLOAT3(-24.f, 0.f, 2.f));

	fog = nullptr;
}

AFog* CWorld::GetFog() const
{
	return fog;
}
