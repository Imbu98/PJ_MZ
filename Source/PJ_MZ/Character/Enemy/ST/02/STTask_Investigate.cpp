#include "STTask_Investigate.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

bool FSTTask_Investigate::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(ControllerHandle);
    return true;
}

EStateTreeRunStatus FSTTask_Investigate::EnterState(
    FStateTreeExecutionContext& Context,
    const FStateTreeTransitionResult& Transition) const
{
    AAIController& Controller = Context.GetExternalData(ControllerHandle);
    APawn* Pawn = Controller.GetPawn();
    if (!Pawn) return EStateTreeRunStatus::Failed;

    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    InstanceData.ElapsedTime = 0.f;

    if (ACharacter* Character = Cast<ACharacter>(Pawn))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
    }

    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_Investigate::Tick(
    FStateTreeExecutionContext& Context,
    const float DeltaTime) const
{
    AAIController& Controller = Context.GetExternalData(ControllerHandle);
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    APawn* Pawn = Controller.GetPawn();
    if (!Pawn) return EStateTreeRunStatus::Failed;

    InstanceData.ElapsedTime += DeltaTime;

    // 20초 초과 → Idle 복귀
    if (InstanceData.ElapsedTime >= InvestigateTime)
    {
        return EStateTreeRunStatus::Succeeded;
    }

    // 도착하면 SoundLocation 근처 랜덤 위치로 계속 이동
    if (Controller.GetMoveStatus() == EPathFollowingStatus::Idle)
    {
        UNavigationSystemV1* NavSystem =
            UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
        if (!NavSystem) return EStateTreeRunStatus::Running;

        FNavLocation NavLocation;
        if (NavSystem->GetRandomReachablePointInRadius(
            InstanceData.SoundLocation, WanderRadius, NavLocation))
        {
            Controller.MoveToLocation(NavLocation.Location, AcceptanceRadius);
        }
    }

    return EStateTreeRunStatus::Running;
}