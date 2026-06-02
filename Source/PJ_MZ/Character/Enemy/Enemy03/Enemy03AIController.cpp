#include "Enemy03AIController.h"

#include "Components/StateTreeAIComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

AEnemy03AIController::AEnemy03AIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemy03AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (AIPerception)
    {
        AIPerception->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
    }

    SetGenericTeamId(FGenericTeamId(1));

    if (InPawn)
    {
        if (UStateTreeAIComponent* STComp = FindComponentByClass<UStateTreeAIComponent>())
        {
            UE_LOG(LogTemp, Warning, TEXT("StateTreeAIComponent 찾음, StartLogic"));
            STComp->StartLogic();
        }
    }
}

bool AEnemy03AIController::IsEnemyLookingAtPlayer() const
{
    APawn* EnemyPawn  = GetPawn();
    if (!EnemyPawn ) return false;

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player) return false;

    // 괴물 → 플레이어 방향
    FVector ToPlayer = (Player->GetActorLocation() - EnemyPawn ->GetActorLocation()).GetSafeNormal();
    FVector EnemyForward = EnemyPawn ->GetActorForwardVector();

    // 시야각 체크
    float DotProduct = FVector::DotProduct(EnemyForward, ToPlayer);
    if (DotProduct < FMath::Cos(FMath::DegreesToRadians(EnemySightAngle))) return false;

    // 장애물 체크
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(EnemyPawn );
    Params.AddIgnoredActor(Player);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        EnemyPawn ->GetActorLocation(),
        Player->GetActorLocation(),
        ECC_Visibility,
        Params
    );

    return !bHit;
}