
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyAIControllerBase.generated.h"

UCLASS()
class PJ_MZ_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIControllerBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BlackboardAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float HearingRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float HearingMaxAge = 5.f;

	// Blackboard 키 이름 - 공통
	const FName BB_SoundLocation = TEXT("SoundLocation");
	const FName BB_TargetActor   = TEXT("TargetActor");
};
