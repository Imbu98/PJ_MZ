

#include "EnemyBase.h"
#include <Components/PicturableComponent.h>
#include "EnemySpawnManager.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PicturableComp = CreateDefaultSubobject<UPicturableComponent>("PicturableComp");
	
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 200.f;
	PawnSensing->SetPeripheralVisionAngle(180.f);
	PawnSensing->bSeePawns = true;
	PawnSensing->bHearNoises = false;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
    PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBase::OnSeePawn);
}

void AEnemyBase::Attack()
{
	// 자식 클래스에서 구체적인 공격 구현
}

void AEnemyBase::OnAttackSuccess()
{
	UE_LOG(LogTemp, Warning, TEXT("공격성공"));
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

void AEnemyBase::OnSeePawn(APawn* SensedPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSeePawn 호출됨"));
    
	if (!SensedPawn) return;
    
	if (SensedPawn->IsPlayerControlled())
	{
		Attack();
	}
}




