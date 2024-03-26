#include "Rendering.h"
#include "../Engine/DirectX/Core/DirectXRenderingEngine.h"

IRenderingInterface::IRenderingInterface()
{
}

IRenderingInterface::~IRenderingInterface()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)
{
	//GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), nullptr);
}

void IRenderingInterface::PostDraw(float DeltaTime)
{
}