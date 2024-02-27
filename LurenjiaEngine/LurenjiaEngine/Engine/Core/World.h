#pragma once
#include "../Core/Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../Actor/Core/Actor.h"
#include "../Actor/Sky/Fog.h"
#include "../EngineType.h"

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

public:
	bool LineTraceSingleByChannel(FHitResult& OutHitResult, const XMFLOAT3& Start, const XMFLOAT3& End) const;
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