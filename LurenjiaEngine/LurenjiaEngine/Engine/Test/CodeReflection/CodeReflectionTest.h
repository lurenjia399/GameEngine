#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "Particles/ParticlePerfStats.h"
#include "Async/TaskGraphInterfaces.h"
#include "RHIDefinitions.h"
#include "PSOPrecache.h"

#include "ParticleSystem.generated.h"

UCLASS()
class UParticleSystem : public UFXSystemAsset
{
	GENERATED_BODY()

	UParticleSystem() :OcclusionBoundsMethod(EPSOBM_ParticleBounds)
	{

	}

	//UPROPERTY()
	const float FPS;

	UPROPERTY()
	const float FPS_const = 20.f;

	UPROPERTY()
	float* FPS_ptr;

	UPROPERTY()
	float& FPS_reference = 20.f;

	UPROPERTY()
	const float* FPS_ConstPoint;

	UPROPERTY()
	const float& FPS_ConstPoint;

	UFUNCTION()
	const UAzureUI3DUserWidget& GetPateWidget();

	UFUNCTION()
	static UAzureUI3DUserWidget* GetPateWidget(FParticleEmitterInstance* Owner,const int32 Offset, float& SpawnTime, const FBaseParticle* ParticleBase, const FVector& OutEndPoint);
	
};

// /**/这种注释里别出现反射宏，暂时还没有去掉


// UPROPERTY()修饰的目前支持的变量：常量，指针，引用，常量指针，常量引用 （都去掉了初始值）
// static 变量不能标记UPROPERTY，和ue保持一致


/*
//UFUNCTION()修饰的目前支持的参数：指针，常量，引用，常量指针，常量引用
//UFUNCTION()修饰的目前支持的返回值：指针，常量指针，常量引用 （返回值的Name默认为ReturnValue）
//UFUNCTION()修饰的目前支持静态的，虚函数，这些都还没有功能，只能能分析出来
*/