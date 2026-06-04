#include "Enemy03Character.h"
#include "Enemy03AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy03Character::AEnemy03Character()
{
	AIControllerClass = AEnemy03AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemy03Character::Attack()
{
	Super::Attack();
	
	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector Dir = Player->GetActorLocation() - GetActorLocation();
		Dir.Z = 0.0;
		if (!Dir.IsNearlyZero())
		{
			SetActorRotation(Dir.Rotation());
		}
	}
	
	HaltMovement();

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->DisableMovement();
	}

	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle, this, &AEnemyBase::OnAttackSuccess,
			AttackMontage->GetPlayLength(), false);
	}
	else
	{
		OnAttackSuccess();
	}
}
