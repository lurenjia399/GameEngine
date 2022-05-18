#include "World.h"
#include "Camera.h"

CWorld::CWorld()
{
	camera = CreateObject<ACamera>("camera");
	camera->SetPosition(XMFLOAT3(-24.f, 0.f, 2.f));
}
