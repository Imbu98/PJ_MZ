#include "STEvaluator_Enemy03.h"
#include "AIController.h"
#include "Enemy03AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"

class AEnemy03AIController;

bool FSTEvaluator_Enemy03::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(ControllerHandle);
    Linker.LinkExternalData(PawnHandle);
    Linker.LinkExternalData(STComponentHandle);

    return true;
}

void FSTEvaluator_Enemy03::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    AAIController& Controller = Context.GetExternalData(ControllerHandle);
    APawn& Pawn = Context.GetExternalData(PawnHandle);

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
    if (!Player) return;

    float Distance = FVector::Dist(Pawn.GetActorLocation(), Player->GetActorLocation());

    // 추적 포기 거리 초과 시 Idle 전환
    if (InstanceData.bPlayerDetected && Distance >= LoseTargetDistance)
    {
        InstanceData.bPlayerDetected = false;
        InstanceData.bPlayerLooking = false;
        InstanceData.TargetActor = nullptr;

        if (AEnemy03AIController* Enemy03Controller = Cast<AEnemy03AIController>(&Controller))
        {
            Enemy03Controller->bPlayerDetected = false;
        }

        UStateTreeAIComponent& STComp = Context.GetExternalData(STComponentHandle);
        FStateTreeEvent Event;
        Event.Tag = FGameplayTag::RequestGameplayTag(FName("Enemy.PlayerLost"));
        STComp.SendStateTreeEvent(Event);
        return;
    }

    if (InstanceData.bPlayerDetected)
    {
        // Chase 중 플레이어 시선 체크
        InstanceData.bPlayerLooking = IsPlayerLookingAtEnemy(Controller);
    }
    else
    {
        // 플레이어가 적을 바라보는 순간 Chase 시작
        bool bLooking = IsPlayerLookingAtEnemy(Controller);
        UE_LOG(LogTemp, Warning, TEXT("bLooking: %s"), bLooking ? TEXT("true") : TEXT("false"));

        if (bLooking)
        {
            InstanceData.bPlayerDetected = true;
            InstanceData.bPlayerLooking = true;

            if (AEnemy03AIController* Enemy03Controller = Cast<AEnemy03AIController>(&Controller))
            {
                Enemy03Controller->bPlayerDetected = true;
            }

            UStateTreeAIComponent& STComp = Context.GetExternalData(STComponentHandle);
            FStateTreeEvent Event;
            Event.Tag = FGameplayTag::RequestGameplayTag(FName("Enemy.PlayerDetected"));
            STComp.SendStateTreeEvent(Event);
        }
    }
}

bool FSTEvaluator_Enemy03::IsPlayerLookingAtEnemy(AAIController& Controller) const
{
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
    APawn* Pawn = Controller.GetPawn();
    if (!Player || !Pawn) return false;

    FVector CameraLocation = Player->GetPawnViewLocation();
    FVector CameraForward = Player->GetControlRotation().Vector();
    FVector ToEnemy = (Pawn->GetActorLocation() - CameraLocation).GetSafeNormal();

    float DotProduct = FVector::DotProduct(CameraForward, ToEnemy);
    float AngleThreshold = FMath::Cos(FMath::DegreesToRadians(LookAtAngleThreshold));

    UE_LOG(LogTemp, Warning, TEXT("DotProduct: %f, AngleThreshold: %f"), DotProduct, AngleThreshold);

    if (DotProduct >= AngleThreshold)
    {
        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Player);
        Params.AddIgnoredActor(Pawn);

        bool bHit = Controller.GetWorld()->LineTraceSingleByChannel(
            HitResult,
            CameraLocation,
            Pawn->GetActorLocation(),
            ECC_Visibility,
            Params
        );

        UE_LOG(LogTemp, Warning, TEXT("bHit: %s, HitActor: %s"),
            bHit ? TEXT("true") : TEXT("false"),
            HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None"));

        if (!bHit)
        {
            return true;
        }
    }

    return false;
}

//
// bool FSTEvaluator_Enemy03::IsPlayerLookingAtEnemy(AAIController& Controller) const
// {
//     ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
//     APawn* Pawn = Controller.GetPawn();
//     if (!Player || !Pawn) return false;
//
//     FVector CameraForward = Player->GetControlRotation().Vector();
//     FVector ToEnemy = (Pawn->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();
//
//     float DotProduct = FVector::DotProduct(CameraForward, ToEnemy);
//     float AngleThreshold = FMath::Cos(FMath::DegreesToRadians(LookAtAngleThreshold));
//
//     if (DotProduct >= AngleThreshold)
//     {
//         FHitResult HitResult;
//         FCollisionQueryParams Params;
//         Params.AddIgnoredActor(Player);
//
//         bool bHit = Controller.GetWorld()->LineTraceSingleByChannel(
//             HitResult,
//             Player->GetActorLocation(),
//             Pawn->GetActorLocation(),
//             ECC_Visibility,
//             Params
//         );
//
//         if (bHit && HitResult.GetActor() == Pawn)
//         {
//             return true;
//         }
//     }
//
//     return false;
// }