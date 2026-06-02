
#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense_Hearing.h"
#include "Character/Enemy/EnemyAIControllerBase.h"
#include "Components/StateTreeAIComponent.h"
#include "Enemy02AIController.generated.h"

struct FActorPerceptionUpdateInfo;

UCLASS()
class PJ_MZ_API AEnemy02AIController : public AEnemyAIControllerBase
{
	GENERATED_BODY()

public:
	AEnemy02AIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	
	
};
