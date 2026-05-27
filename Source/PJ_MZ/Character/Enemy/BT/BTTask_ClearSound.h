
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearSound.generated.h"

UCLASS()
class PJ_MZ_API UBTTask_ClearSound : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearSound();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SoundLocationKey;
};
