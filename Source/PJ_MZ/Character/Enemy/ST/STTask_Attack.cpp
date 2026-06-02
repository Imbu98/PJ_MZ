#include "STTask_Attack.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"

bool FSTTask_Attack::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_Attack::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return EStateTreeRunStatus::Failed;

	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn))
	{
		Enemy->Attack();
	}

	return EStateTreeRunStatus::Succeeded;
}