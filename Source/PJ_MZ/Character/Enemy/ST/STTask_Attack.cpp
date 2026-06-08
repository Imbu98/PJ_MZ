#include "STTask_Attack.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Enemy/Enemy03/Enemy03Character.h"
#include "Kismet/GameplayStatics.h"

bool FSTTask_Attack::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_Attack::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: ATTACK 시작"));
	
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}
	

	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn))
	{
        Enemy->StartAttack();
	}

    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Attack::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	AAIController& Controller =
		Context.GetExternalData(ControllerHandle);

	APawn* Pawn = Controller.GetPawn();
	if (!Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn);
	if (!Enemy)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (!Enemy->IsAttacking())
	{
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void FSTTask_Attack::ExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller =
		Context.GetExternalData(ControllerHandle);

	if (AEnemyBase* Enemy =
		Cast<AEnemyBase>(Controller.GetPawn()))
	{
		Enemy->StopAttack();
	}
}