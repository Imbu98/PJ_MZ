

#include "STTask_FreezePlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

bool FSTTask_FreezePlayer::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_FreezePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return EStateTreeRunStatus::Failed;

	// 이동 완전 정지
	Controller.StopMovement();

	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = FrozenSpeed;
	}

	return EStateTreeRunStatus::Running;
}

void FSTTask_FreezePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return;

	// 이동 속도 복구
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}
