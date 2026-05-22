#include "BTDecorator_CanHear.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanHear::UBTDecorator_CanHear()
{
	NodeName = TEXT("Can Hear");
}

bool UBTDecorator_CanHear::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return false;

	// SoundLocation이 ZeroVector가 아니면 소리를 감지한 것
	FVector SoundLocation = Blackboard->GetValueAsVector(SoundLocationKey.SelectedKeyName);
	return SoundLocation != FVector::ZeroVector;
}