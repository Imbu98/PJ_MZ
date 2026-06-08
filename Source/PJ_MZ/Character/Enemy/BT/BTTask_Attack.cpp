#include "BTTask_Attack.h"
#include "AIController.h"
#include "../EnemyBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(OwnerComp.GetWorld(), 0);
	if (!Player) return EBTNodeResult::Failed;

	// 플레이어와 거리 체크
	float Distance = FVector::Dist(Enemy->GetActorLocation(), Player->GetActorLocation());
	if (Distance > AttackRange) return EBTNodeResult::Failed;

	// 공격 실행
	Enemy->StartAttack();

	return EBTNodeResult::Succeeded;
}
