
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyAIControllerBase.h"
#include "Enemy02AIController.generated.h"

UCLASS()
class PJ_MZ_API AEnemy02AIController : public AEnemyAIControllerBase
{
	GENERATED_BODY()

public:
	AEnemy02AIController();

protected:
	virtual FString GetBehaviorTreePath() const override
	{ return TEXT("/Game/SY/Enemy/Enemy02/BT_Enemy02"); }

	virtual FString GetBlackboardPath() const override
	{ return TEXT("/Game/SY/Enemy/Enemy02/BB_Enemy02"); }

	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
