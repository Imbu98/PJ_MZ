#include "STTask_Stun.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

bool FSTTask_Stun::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_Stun::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.StunTimer = 0.f;

	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	Controller.StopMovement();

	if (ACharacter* Character = Cast<ACharacter>(Controller.GetPawn()))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy Stun 시작"));
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Stun::Tick(
	FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.StunTimer += DeltaTime;

	if (InstanceData.StunTimer >= StunDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Stun 종료"));
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void FSTTask_Stun::ExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	AAIController& Controller = Context.GetExternalData(ControllerHandle);

	if (ACharacter* Character = Cast<ACharacter>(Controller.GetPawn()))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}