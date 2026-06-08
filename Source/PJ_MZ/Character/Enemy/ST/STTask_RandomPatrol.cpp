
#include "STTask_RandomPatrol.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Character/Enemy/Enemy02/Enemy02Character.h"
#include "Character/Enemy/Enemy03/Enemy03Character.h"

bool FSTTask_RandomPatrol::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_RandomPatrol::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	// UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: idle 시작"));
	
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return EStateTreeRunStatus::Failed;

	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	}
	
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Controller.GetPawn()))
	{
		Enemy->StartAmbientSounds();
	}
	if (AEnemy02Character* Enemy = Cast<AEnemy02Character>(Controller.GetPawn()))
	{
		Enemy->BreathAudioComp->Stop();
		Enemy->StartAmbientSounds();
	}
	if (AEnemy03Character* E3 = Cast<AEnemy03Character>(Controller.GetPawn()))
	{
		E3->SetAlerted(false);
		E3->ChaseAudioComp->Stop();
	}
	
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Controller.GetWorld());
	if (!NavSystem) return EStateTreeRunStatus::Failed;

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), PatrolRadius, RandomLocation))
	{
		InstanceData.TargetLocation = RandomLocation.Location;
		Controller.MoveToLocation(RandomLocation.Location, AcceptanceRadius);
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_RandomPatrol::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (Controller.GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		InstanceData.WaitTimer += DeltaTime;
		if (InstanceData.WaitTimer >= WaitTime)
		{
			InstanceData.WaitTimer = 0.f;
			return EStateTreeRunStatus::Succeeded;
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: idle 끝"));

	return EStateTreeRunStatus::Running;
}

