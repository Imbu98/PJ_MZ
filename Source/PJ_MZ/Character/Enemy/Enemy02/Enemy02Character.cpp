

#include "Enemy02Character.h"


void AEnemy02Character::Attack()
{
	// 플레이어에게 스턴 적용
	
	// 부모의 OnAttackSuccess 호출해서 사라지기 처리
	Super::Attack();
	OnAttackSuccess();
}




