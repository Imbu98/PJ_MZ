#include "BTTask_ClearSound.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearSound::UBTTask_ClearSound()
{
	NodeName = TEXT("Clear Sound");
}

EBTNodeResult::Type UBTTask_ClearSound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	UE_LOG(LogTemp, Warning, TEXT("ClearSound 실행됨"));

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(SoundLocationKey.SelectedKeyName, FVector::ZeroVector);
	Blackboard->SetValueAsBool(TEXT("bHearSound"), false);

	return EBTNodeResult::Succeeded;
}