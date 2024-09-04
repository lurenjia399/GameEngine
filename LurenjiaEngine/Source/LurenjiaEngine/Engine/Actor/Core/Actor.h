#pragma once

#ifndef Actor
#define Actor
#include "../../LurenjiaEngine.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../../Component/TransformationComponent.h"
#include "Actor.generated.h"

UCLASS()
class AActor : public CCoreMinimalObject
{
	GENERATED_BODY()
public:
	AActor();

	virtual void SetPosition(const XMFLOAT3& InPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetScale(const XMFLOAT3& InScale);

	virtual XMFLOAT3 GetPosition() const;
	virtual fvector_3d GetRotation() const;
	virtual XMFLOAT3 GetScale() const;

	virtual void SetForward(const XMFLOAT3& Forward) const;
	virtual void SetRight(const XMFLOAT3& Right) const;
	virtual void SetUp(const XMFLOAT3& Up) const;

	virtual XMFLOAT3 GetForward() const;
	virtual XMFLOAT3 GetRight() const;
	virtual XMFLOAT3 GetUp() const;

	virtual FORCEINLINE CTransformationComponent* GetRootComponent() { return RootComponent; }

	virtual void SetPickup(bool InPickup);

	virtual void SetVisible(bool InVisible);
	virtual bool GetVisible();
protected:

	UPROPERTY()
	CTransformationComponent* RootComponent;

	UPROPERTY()
	float test_float;

	UPROPERTY()
	int test_int;

	UPROPERTY()
	bool test_bool;

	UPROPERTY()
	string test_string;

	UPROPERTY()
	XMFLOAT3 test_xmfloat3;

	UPROPERTY()
	std::map<string, int> testmap;

	UPROPERTY()
	std::vector<int> testvector;
};

#endif