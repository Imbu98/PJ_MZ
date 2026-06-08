

#include "EnemyBase.h"
#include <Components/PicturableComponent.h>
#include "EnemySpawnManager.h"
#include "AIController.h"
#include "Character/Player/HT_Player.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PicturableComp = CreateDefaultSubobject<UPicturableComponent>("PicturableComp");
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::HaltMovement()
{
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->StopMovement();
	}
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->StopMovementImmediately();
	}
}

void AEnemyBase::Attack()
{
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

	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, AttackSound, GetActorLocation(),
			FRotator::ZeroRotator,
			1.f, 1.f, 0.f,
			SoundAttenuation);
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
	
	// 자식 클래스에서 구체적인 공격 구현
	
	
}

void AEnemyBase::OnAttackSuccess()
{
	UE_LOG(LogTemp, Warning, TEXT("공격성공"));
	
	AHT_Player* player =Cast<AHT_Player> (GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player)
	{
		player->playerAttacked(10.f,1.5f);
	}
	
	DistoryAndRequestRespawn();
}

void AEnemyBase::DistoryAndRequestRespawn()
{
	UE_LOG(LogTemp, Warning, TEXT("액터 숨기기 및 재스폰 요청"));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	AEnemySpawnManager* SpawnManager = Cast<AEnemySpawnManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnManager::StaticClass())
	);

	if (SpawnManager)
	{
		SpawnManager->RequestRespawn(this, RespawnDelay);
	}
	
	Destroy();
}

void AEnemyBase::StartAmbientSounds()
{
	if (AmbientSounds.Num() == 0) return;

	const float Delay = FMath::RandRange(5.f, 12.f);
	GetWorldTimerManager().SetTimer(
		AmbientTimerHandle, this,
		&AEnemyBase::PlayRandomAmbientSound,
		Delay, false);
}

void AEnemyBase::StopAmbientSounds()
{
	GetWorldTimerManager().ClearTimer(AmbientTimerHandle);
}

void AEnemyBase::PlayRandomAmbientSound()
{
	if (AmbientSounds.Num() > 0)
	{
		const int32 Idx = FMath::RandRange(0, AmbientSounds.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(
			this, AmbientSounds[Idx], GetActorLocation(),
			FRotator::ZeroRotator,
			1.f, 1.f, 0.f,
			SoundAttenuation);
	}

	const float Delay = FMath::RandRange(5.f, 12.f);
	GetWorldTimerManager().SetTimer(
		AmbientTimerHandle, this,
		&AEnemyBase::PlayRandomAmbientSound,
		Delay, false);
}



