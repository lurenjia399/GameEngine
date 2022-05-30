#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../Actor/Core/Actor.h"

class ACamera;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	ACamera* GetCamera()const { return camera; }

	template<typename T>
	T* CreateActor(const string& name);
private:
	CVARIABLE()
	ACamera* camera;

	//世界中的actor
	CVARIABLE()
	vector<AActor*> WorldActors;
};

template<typename T>
T* CWorld::CreateActor(const string& name)
{
	T* actor = CreateObject<T>(name);
	WorldActors.emplace_back(actor);

	return actor;
}