#include "Enemy03AIController.h"

#include "Components/StateTreeAIComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
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

    // State Tree 수동 시작
    if (UStateTreeAIComponent* STComp = FindComponentByClass<UStateTreeAIComponent>())
    {
        STComp->StartLogic();
        UE_LOG(LogTemp, Warning, TEXT("StateTree StartLogic 호출됨"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("STComp 없음"));
    }
}

bool AEnemy03AIController::IsPlayerLookingAtMe() const
{
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player || !GetPawn()) return false;

    // 플레이어 카메라 방향
    FVector CameraForward = Player->GetControlRotation().Vector();

    // 플레이어에서 적 방향
    FVector ToEnemy = (GetPawn()->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal();

    // DotProduct로 각도 계산
    float DotProduct = FVector::DotProduct(CameraForward, ToEnemy);
    float AngleThreshold = FMath::Cos(FMath::DegreesToRadians(LookAtAngleThreshold));

    if (DotProduct >= AngleThreshold)
    {
        // 시야가 가리는지 LineTrace 체크
        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Player);

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Player->GetActorLocation(),
            GetPawn()->GetActorLocation(),
            ECC_Visibility,
            Params
        );

        // 라인트레이스가 적에 맞으면 바라보고 있는 것
        if (bHit && HitResult.GetActor() == GetPawn())
        {
            return true;
        }
    }

    return false;
}
