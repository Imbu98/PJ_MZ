
#include "BTTask_RandomPatrol.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class UCharacterMovementComponent;

UBTTask_RandomPatrol::UBTTask_RandomPatrol()
{
	NodeName = TEXT("Random Patrol");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	if (UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(
		Cast<ACharacter>(Pawn)->GetMovementComponent()))
	{
		Movement->MaxWalkSpeed = 300.f;
	}
	
	FNavLocation RandomLocation;
	bool bFound = NavSystem->GetRandomReachablePointInRadius(
		Pawn->GetActorLocation(),
		PatrolRadius,
		RandomLocation
	);

	if (!bFound) return EBTNodeResult::Failed;

	AIController->MoveToLocation(RandomLocation.Location, AcceptanceRadius);

	return EBTNodeResult::InProgress;
	// return EBTNodeResult::Succeeded;
}

void UBTTask_RandomPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		// 도착 후 1~2초 대기
		WaitTimer += DeltaSeconds;
		if (WaitTimer >= WaitTime)
		{
			WaitTimer = 0.f;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
