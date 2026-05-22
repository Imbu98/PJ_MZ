#include "BTTask_MoveToSound.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToSound::UBTTask_MoveToSound()
{
	NodeName = TEXT("Move To Sound");
}

EBTNodeResult::Type UBTTask_MoveToSound::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	// Blackboard에서 소리 위치 읽기
	FVector SoundLocation = Blackboard->GetValueAsVector(SoundLocationKey.SelectedKeyName);

	if (SoundLocation == FVector::ZeroVector) return EBTNodeResult::Failed;

	// 소리 위치로 이동
	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(
		SoundLocation,
		AcceptanceRadius
	);

	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}