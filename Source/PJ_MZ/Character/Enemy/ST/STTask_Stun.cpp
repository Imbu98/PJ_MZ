#include "STTask_Stun.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "AIController.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Enemy/Enemy03/Enemy03AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

bool FSTTask_Stun::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_Stun::EnterState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	// UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: STUN 시작"));

	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.StunTimer = 0.f;

	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	Controller.StopMovement();

	if (ACharacter* Character = Cast<ACharacter>(Controller.GetPawn()))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Controller.GetPawn()))
	{
		Enemy->HaltMovement();

		if (Enemy->StunMontage)
		{
			Enemy->PlayAnimMontage(Enemy->StunMontage);
		}

		if (Enemy->StunSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				Enemy, Enemy->StunSound, Enemy->GetActorLocation(),
				FRotator::ZeroRotator,
				1.f, 1.f, 0.f,
				Enemy->SoundAttenuation);
		}
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
	
		AAIController& Controller = Context.GetExternalData(ControllerHandle);
		UStateTreeAIComponent* STComp =
			   Controller.FindComponentByClass<UStateTreeAIComponent>();
		
		if (AEnemy03AIController* Enemy03 = Cast<AEnemy03AIController>(&Controller))
		{
			FStateTreeEvent Event;
			Event.Tag = Enemy03->IsEnemyLookingAtPlayer()
				? FGameplayTag::RequestGameplayTag("Enemy.StunEndedChase")
				: FGameplayTag::RequestGameplayTag("Enemy.StunEndedIdle");

			Enemy03->bResetDetection = true;
			UE_LOG(LogTemp, Warning, TEXT("bResetDetection = true 설정됨"));
			
			STComp->SendStateTreeEvent(Event);

			UE_LOG(LogTemp, Warning,
				TEXT("Send Event : %s"),
				*Event.Tag.ToString());

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy03AIController 캐스트 실패!"));
		}
			return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}

void FSTTask_Stun::ExitState(
	FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy03 Current State: STUN 끝"));
	
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	
	
	UStateTreeAIComponent* STComp =
		   Controller.FindComponentByClass<UStateTreeAIComponent>();
	if (!STComp) return;
		
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Controller.GetPawn()))
	{
		if (UAnimInstance* Anim = Enemy->GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Stop(0.1f);
		}
	}
}