#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../Actor/Core/Actor.h"
#include "../Actor/Sky/Fog.h"

class ACamera;
class AFog;

class CWorld : public CCoreMinimalObject
{
public:
	CWorld();
	ACamera* GetCamera() const { return camera; }
	AFog* GetFog() const;

	template<typename T>
	T* CreateActor(const string& name);
private:
	CVARIABLE()
	ACamera* camera;

	CVARIABLE()
	AFog* fog;

	//�����е�actor
	CVARIABLE()
	vector<AActor*> WorldActors;
};

template<typename T>
T* CWorld::CreateActor(const string& name)
{
	T* actor = LurenjiaEngine::CreateObject<T>(name);
	WorldActors.emplace_back(actor);
	//��ʱ�����������ڴ������ʱ�򣬽����浽������
	if (typeid(AFog) == typeid(*actor))
	{
		fog = (AFog*)actor;
	}

	return actor;
}