#include "Enemy03Character.h"
#include "Enemy03AIController.h"

AEnemy03Character::AEnemy03Character()
{
	AIControllerClass = AEnemy03AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	if (PawnSensing)
	{
		PawnSensing->SightRadius = 150.f;
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
	UE_LOG(LogTemp, Warning, TEXT("SensedPawn : 공격!"));

	Attack();
}
