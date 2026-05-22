
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanHear.generated.h"

UCLASS()
class PJ_MZ_API UBTDecorator_CanHear : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CanHear();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// Blackboard에서 읽어올 소리 위치 키
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SoundLocationKey;
};
