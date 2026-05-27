#include "BTTask_Investigate.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Investigate::UBTTask_Investigate()
{
    NodeName = TEXT("Investigate");
    bNotifyTick = true;
    ElapsedTime = 0.f;
}

EBTNodeResult::Type UBTTask_Investigate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ElapsedTime = 0.f;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    FVector SoundLocation = Blackboard->GetValueAsVector(SoundLocationKey.SelectedKeyName);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    if (UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(
    Cast<ACharacter>(AIController->GetPawn())->GetMovementComponent()))
    {
        Movement->MaxWalkSpeed = 600.f;
    }
    
    FNavLocation RandomLocation;
    if (NavSystem->GetRandomReachablePointInRadius(SoundLocation, InvestigateRadius, RandomLocation))
    {
        AIController->MoveToLocation(RandomLocation.Location, AcceptanceRadius);
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_Investigate::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ElapsedTime += DeltaSeconds;

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("BT_Investigate : AIController 유효하지않음"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("BT_Investigate : Pawn 유효하지않음"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 플레이어 거리 체크
    if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        float Distance = FVector::Dist(Pawn->GetActorLocation(), Player->GetActorLocation());
        if (Distance <= AttackRange)
        {
            // 공격 범위 안에 있으면 공격
            if (AEnemyBase* Enemy = Cast<AEnemyBase>(Pawn))
            {
                Enemy->Attack();
            }
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }

    // 이동 완료 시 다음 랜덤 위치로 이동
    if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
    {
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

        if (NavSystem && Blackboard)
        {
            FVector SoundLocation = Blackboard->GetValueAsVector(SoundLocationKey.SelectedKeyName);
            FNavLocation RandomLocation;
            if (NavSystem->GetRandomReachablePointInRadius(SoundLocation, InvestigateRadius, RandomLocation))
            {
                AIController->MoveToLocation(RandomLocation.Location, AcceptanceRadius);
            }
        }
    }

    // 시간 초과 시 Idle 복귀
    if (ElapsedTime >= InvestigateTime)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}