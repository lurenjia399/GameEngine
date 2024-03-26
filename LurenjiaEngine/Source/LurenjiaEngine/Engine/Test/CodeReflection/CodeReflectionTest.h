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

// /**/����ע�������ַ���꣬��ʱ��û��ȥ��


// UPROPERTY()���ε�Ŀǰ֧�ֵı�����������ָ�룬���ã�����ָ�룬�������� ����ȥ���˳�ʼֵ��
// static �������ܱ��UPROPERTY����ue����һ��


/*
//UFUNCTION()���ε�Ŀǰ֧�ֵĲ�����ָ�룬���������ã�����ָ�룬��������
//UFUNCTION()���ε�Ŀǰ֧�ֵķ���ֵ��ָ�룬����ָ�룬�������� ������ֵ��NameĬ��ΪReturnValue��
//UFUNCTION()���ε�Ŀǰ֧�־�̬�ģ��麯������Щ����û�й��ܣ�ֻ���ܷ�������
*/