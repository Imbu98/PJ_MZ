#include "Enemy02Character.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy02AIController.h"

AEnemy02Character::AEnemy02Character()
{
	AIControllerClass = AEnemy02AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemy02Character::Attack()
{
	// 플레이어에게 스턴 적용
	
	// 부모의 OnAttackSuccess 호출해서 사라지기 처리
	Super::Attack();
	OnAttackSuccess();
}




