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
	shared_ptr<ACamera> GetCamera() const { return camera; }
	shared_ptr<AFog> GetFog() const;

	template<typename T>
	shared_ptr<T> CreateActor(const string& name)
	{
		shared_ptr<T> actor = LurenjiaEngine::CreateObject<T>(shared_from_this(), name);
		WorldActors.emplace_back(actor);

		return actor;
	}

	template<>
	shared_ptr<AFog> CreateActor(const string& name)
	{
		fog = LurenjiaEngine::CreateObject<AFog>(shared_from_this(), name);
		WorldActors.emplace_back(fog);

		return fog;
	}

public:
	bool LineTraceSingleByChannel(FHitResult& OutHitResult, const XMFLOAT3& Start, const XMFLOAT3& End) const;
private:
	CVARIABLE()
	std::shared_ptr<ACamera> camera;

	CVARIABLE()
	std::shared_ptr<AFog> fog;

	//世界中的actor
	CVARIABLE()
	vector<shared_ptr<AActor>> WorldActors;
};

