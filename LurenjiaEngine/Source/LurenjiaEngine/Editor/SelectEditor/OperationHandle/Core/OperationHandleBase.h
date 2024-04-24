#pragma once
#include "../Engine/Actor/Core/Actor.h"
#include "../Engine/Interface/DirectXDeviceInterface.h"

class CCustomMeshComponent;
class AOperationHandleBase : public AActor, public IDirectXDeviceInterface
{
	typedef AActor Super;
public:
	AOperationHandleBase();
protected:
	std::shared_ptr<CCustomMeshComponent> XAxisComponent;
	std::shared_ptr<CCustomMeshComponent> YAxisComponent;
	std::shared_ptr<CCustomMeshComponent> ZAxisComponent;
};