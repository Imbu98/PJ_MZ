#include "Enemy03Character.h"
#include "Enemy03AIController.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy03Character::AEnemy03Character()
{
	AIControllerClass = AEnemy03AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	ChaseAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ChaseAudio"));
	ChaseAudioComp->SetupAttachment(RootComponent);
	ChaseAudioComp->bAutoActivate = false;
	ChaseAudioComp->bOverrideAttenuation = true;
}

void AEnemy03Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (SoundAttenuation && ChaseAudioComp)
	{
		ChaseAudioComp->AttenuationSettings = SoundAttenuation;
	}
}

// void AEnemy03Character::Attack()
// {
// 	Super::StartAttack();
// 	
// 	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
// 	{
// 		FVector Dir = Player->GetActorLocation() - GetActorLocation();
// 		Dir.Z = 0.0;
// 		if (!Dir.IsNearlyZero())
// 		{
// 			SetActorRotation(Dir.Rotation());
// 		}
// 	}
// 	
// 	HaltMovement();
//
// 	if (UCharacterMovementComponent* Move = GetCharacterMovement())
// 	{
// 		Move->DisableMovement();
// 	}
//
// 	if (AttackMontage)
// 	{
// 		PlayAnimMontage(AttackMontage);
//
// 		FTimerHandle TimerHandle;
// 		GetWorldTimerManager().SetTimer(
// 			TimerHandle, this, &AEnemyBase::OnAttackSuccess,
// 			AttackMontage->GetPlayLength(), false);
// 	}
// 	else
// 	{
// 		OnAttackSuccess();
// 	}
// }