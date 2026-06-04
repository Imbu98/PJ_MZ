

#include "STTask_FreezePlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Character/Enemy/Enemy03/Enemy03Character.h"

bool FSTTask_FreezePlayer::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_FreezePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	// UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: FREEZE 끝"));
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn) return EStateTreeRunStatus::Failed;

	if (AEnemy03Character* Enemy = Cast<AEnemy03Character>(Controller.GetPawn()))
	{
		Enemy->SetAlerted(true);
	}
	
	Controller.StopMovement();

	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = FrozenSpeed;
	}

	return EStateTreeRunStatus::Running;
}

void FSTTask_FreezePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	// UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: FREEZE 시작"));

	// AAIController& Controller = Context.GetExternalData(ControllerHandle);
	// APawn* Pawn = Controller.GetPawn();
	// if (!Pawn) return;
	//
	// if (ACharacter* Character = Cast<ACharacter>(Pawn))
	// {
	// 	Character->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	// }
}
