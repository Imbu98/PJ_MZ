
#include "STTask_ChasePlayer.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Character/Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

class AEnemyBase;

bool FSTTask_ChasePlayer::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(ControllerHandle);
	return true;
}

EStateTreeRunStatus FSTTask_ChasePlayer::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogTemp, Warning, TEXT("쫒는모드 On"));
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("폰 존재 X"));
		return EStateTreeRunStatus::Failed;
	}
	// 이동 속도 빠르게
	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("이동속도업"));
		Character->GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_ChasePlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	UE_LOG(LogTemp, Warning, TEXT("쫒는모드 틱"));
	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	APawn* Pawn = Controller.GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("틱 폰 없음"));
		return EStateTreeRunStatus::Failed;
	}
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("틱 플레이어없음"));
		return EStateTreeRunStatus::Running;
	}
	// 플레이어 쪽으로 이동
	Controller.MoveToActor(Player, AttackRange);

	// 공격 범위 안에 있으면 공격
	float Distance = FVector::Dist(Pawn->GetActorLocation(), Player->GetActorLocation());
	if (Distance <= AttackRange)
	{
		if (AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn))
		{
			Enemy->Attack();
		}
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void FSTTask_ChasePlayer::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UE_LOG(LogTemp, Warning, TEXT("쫒는모드 종료"));

	AAIController& Controller = Context.GetExternalData(ControllerHandle);
	Controller.StopMovement();
}
