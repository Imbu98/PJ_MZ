#include "Enemy03Character.h"
#include "Enemy03AIController.h"

AEnemy03Character::AEnemy03Character()
{
	AIControllerClass = AEnemy03AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemy03Character::Attack()
{
	Super::Attack();
	OnAttackSuccess();
}
