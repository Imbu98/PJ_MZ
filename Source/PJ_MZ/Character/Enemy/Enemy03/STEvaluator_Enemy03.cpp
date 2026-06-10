#include "STEvaluator_Enemy03.h"
#include "AIController.h"
#include "Enemy03AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Character/Enemy/EnemyBase.h"
#include "Components/CapsuleComponent.h"

class AEnemy03AIController;

bool FSTEvaluator_Enemy03::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(AIControllerHandle);
    Linker.LinkExternalData(PawnHandle);
    Linker.LinkExternalData(STComponentHandle);

    return true;
}

void FSTEvaluator_Enemy03::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    AAIController& Controller = Context.GetExternalData(AIControllerHandle);
    APawn& Pawn = Context.GetExternalData(PawnHandle);
    UStateTreeAIComponent& STComp = Context.GetExternalData(STComponentHandle);
    
    if (AEnemy03AIController* Enemy =
    Cast<AEnemy03AIController>(&Controller))
    {
        if (Enemy->bResetDetection)
        {
            InstanceData.bPlayerDetected = false;
            InstanceData.bPlayerLooking = false;
            InstanceData.bWasInAttackRange = false;
            InstanceData.TargetActor = nullptr;

            Enemy->bResetDetection = false;

        UE_LOG(LogTemp, Warning, TEXT("Detection Reset 완료"))
        }
    }
    
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
    if (!Player) return;
    
    InstanceData.AttackRange = AttackRange;
    
    float Distance = FVector::Dist(Pawn.GetActorLocation(), Player->GetActorLocation()); 
    
    if (Distance <= AttackRange && !InstanceData.bWasInAttackRange)
    {
        InstanceData.bWasInAttackRange = true;
    
        FStateTreeEvent Event;
        Event.Tag = FGameplayTag::RequestGameplayTag("Enemy.Attack");
        STComp.SendStateTreeEvent(Event);
        return;
    }
    if (Distance > AttackRange)
    {
        InstanceData.bWasInAttackRange = false;
    }
    

    if (InstanceData.bPlayerDetected && Distance > LoseTargetDistance)
    {
        InstanceData.bPlayerDetected = false;
        InstanceData.bPlayerLooking = false;
        InstanceData.bWasInAttackRange = false;
        InstanceData.TargetActor = nullptr;
        
        FStateTreeEvent Event;
        Event.Tag = FGameplayTag::RequestGameplayTag(FName("Enemy.PlayerLost"));
        STComp.SendStateTreeEvent(Event);
        return;
    }
    
    if (Distance <= LoseTargetDistance)
    {
        if (InstanceData.bPlayerDetected)
        {
            InstanceData.bPlayerLooking = IsPlayerLookingAtEnemy(Controller);
            InstanceData.TargetActor = Player;
    
            return;
        }
    
        if (IsPlayerLookingAtEnemy(Controller))
        {
            InstanceData.bPlayerDetected = true;
            InstanceData.bPlayerLooking = true;
            InstanceData.TargetActor = Player;
        
            UE_LOG(LogTemp, Warning, TEXT("Enemy03: PlayerDetected 이벤트 발송"));
    
            FStateTreeEvent Event;
            Event.Tag = FGameplayTag::RequestGameplayTag(FName("Enemy.PlayerDetected"));
            STComp.SendStateTreeEvent(Event);
            
            return;
        }
    }
}
bool FSTEvaluator_Enemy03::IsPlayerLookingAtEnemy(AAIController& Controller) const
{

    UWorld* World = Controller.GetWorld();
    if (!World) return false;
    
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC) return false;
    
    ACharacter* Player = Cast<ACharacter>(PC->GetPawn());
    if (!Player) return false;
    
    ACharacter* Enemy = Cast<ACharacter>(Controller.GetPawn());
    if (!Enemy) return false;

    UCapsuleComponent* Capsule = Enemy->GetCapsuleComponent();
    if (!Capsule) return false;
    
    int32 SizeX = 0;
    int32 SizeY = 0;
    PC->GetViewportSize(SizeX, SizeY);
    
    if (SizeX <= 0 || SizeY <= 0) return false;
    
    const FVector CapsuleCenter = Capsule->GetComponentLocation();
    const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
    const float Radius = Capsule->GetScaledCapsuleRadius();
    
    const FVector Up = Capsule->GetUpVector();
    const FVector Right = Capsule->GetRightVector();
    const FVector Forward = Capsule->GetForwardVector();
    
    const FVector Points[] =
    {
        CapsuleCenter,
        CapsuleCenter + Up * HalfHeight,
        CapsuleCenter - Up * HalfHeight,
        CapsuleCenter + Right * Radius,
        CapsuleCenter - Right * Radius,
        CapsuleCenter + Forward * Radius,
        CapsuleCenter - Forward * Radius,
    };
            
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Player);
    Params.AddIgnoredActor(Enemy);
    
    const FVector TraceStart = Player->GetPawnViewLocation();
    const float ScreenMaxX = static_cast<float>(SizeX);
    const float ScreenMaxY = static_cast<float>(SizeY);
    
    
    for (const FVector& Point : Points)
    {
        FVector2D ScreenPos;
        
        if (!PC->ProjectWorldLocationToScreen(Point, ScreenPos))
            continue;

        if (ScreenPos.X < 0.0f || ScreenPos.X > ScreenMaxX ||
            ScreenPos.Y < 0.0f || ScreenPos.Y > ScreenMaxY) continue;
        
        if (!World->LineTraceTestByChannel(TraceStart, Point, ECC_Visibility, Params))
        {
            return true;
        }
    }
    return false;
}