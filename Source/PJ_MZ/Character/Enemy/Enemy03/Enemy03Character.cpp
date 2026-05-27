#include "Enemy03Character.h"
#include "Enemy03AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "Components/StateTreeComponent.h"

AEnemy03Character::AEnemy03Character()
{
	AIControllerClass = AEnemy03AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	UE_LOG(LogTemp, Warning, TEXT("PawnSensing: %s"), PawnSensing ? TEXT("유효") : TEXT("NULL"));
    
	if (PawnSensing)
	{
		PawnSensing->SightRadius = 200.f;
		PawnSensing->SetPeripheralVisionAngle(180.f);
		PawnSensing->bSeePawns = true;
		PawnSensing->bHearNoises = false;
		PawnSensing->SensingInterval = 0.1f; 
	}
}

void AEnemy03Character::Attack()
{
	Super::Attack();
	OnAttackSuccess();
}

void AEnemy03Character::OnSeePawn(APawn* SensedPawn)
{
	if (!SensedPawn) return;
	if (!SensedPawn->IsPlayerControlled()) return;
	UE_LOG(LogTemp, Warning, TEXT("플레이어 일정 거리 이상 진입 공격!"));

	Attack();
}
