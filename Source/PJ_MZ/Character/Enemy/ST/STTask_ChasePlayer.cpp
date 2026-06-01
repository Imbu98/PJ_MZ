
#include "STTask_ChasePlayer.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

class AEnemyBase;

bool FSTTask_ChasePlayer::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_ChasePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return EStateTreeRunStatus::Failed;
	
	if (ACharacter* Character = Cast<ACharacter>(Pawn)) 
		Character->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_ChasePlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return EStateTreeRunStatus::Failed;
	
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
	if (!Player) return EStateTreeRunStatus::Running;
	
	float Distance = FVector::Dist(Pawn->GetActorLocation(), Player->GetActorLocation());
	if (Distance <= AttackRange)
	{
		if (AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn))
		{
			Controller.StopMovement();
			Enemy->Attack();
		}
		return EStateTreeRunStatus::Succeeded;
	}
	
	const float AcceptanceRadius = AttackRange * 0.5f;
	Controller.MoveToActor(Player, AcceptanceRadius);
	
	return EStateTreeRunStatus::Running;
}

void FSTTask_ChasePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	Controller.StopMovement();
}
