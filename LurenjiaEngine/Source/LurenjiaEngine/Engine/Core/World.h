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
	T* CreateActor(const string& name)
	{
		T* actor = LurenjiaEngine::CreateObject<T>(this, name);
		WorldActors.emplace_back(actor);

		return actor;
	}

	template<>
	AFog* CreateActor(const string& name)
	{
		fog = LurenjiaEngine::CreateObject<AFog>(this, name);
		WorldActors.emplace_back(fog);

		return fog;
	}

	const vector<AActor*> GetWorldActors() const {return WorldActors;}

public:
	bool LineTraceSingleByChannel(FHitResult& OutHitResult, const XMFLOAT3& Start, const XMFLOAT3& End) const;
private:
	ACamera* camera;

	AFog* fog;

	//世界中的actor
	vector<AActor*> WorldActors;
};

