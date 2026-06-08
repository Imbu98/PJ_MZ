#include "STTask_MoveToSound.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "NavigationSystem.h"
#include "Character/Enemy/Enemy02/Enemy02Character.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

bool FSTTask_MoveToSound::Link(FStateTreeLinker& Linker)
{
    Linker.LinkExternalData(ControllerHandle);
    return true;
}

EStateTreeRunStatus FSTTask_MoveToSound::EnterState(
    FStateTreeExecutionContext& Context,
    const FStateTreeTransitionResult& Transition) const
{
    AAIController& Controller = Context.GetExternalData(ControllerHandle);
    APawn* Pawn = Controller.GetPawn();
    if (!Pawn) return EStateTreeRunStatus::Failed;

    if (ACharacter* Character = Cast<ACharacter>(Pawn))
    {
        Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
    }

    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    if (InstanceData.SoundLocation != FVector::ZeroVector)
    {
        Controller.MoveToLocation(InstanceData.SoundLocation, AcceptanceRadius);
    }
    
    if (AEnemy02Character* Enemy = Cast<AEnemy02Character>(Controller.GetPawn()))
    {
        Enemy->StopAmbientSounds();
        Enemy->BreathAudioComp->Play();  // 숨소리 시작

        if (Enemy->DetectionSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                Enemy, Enemy->DetectionSound, Enemy->GetActorLocation(),
                FRotator::ZeroRotator,
                1.f, 1.f, 0.f,
                Enemy->SoundAttenuation);
        }
    }

    return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FSTTask_MoveToSound::Tick(
    FStateTreeExecutionContext& Context,
    const float DeltaTime) const
{
    AAIController& Controller = Context.GetExternalData(ControllerHandle);
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
    APawn* Pawn = Controller.GetPawn();
    if (!Pawn) return EStateTreeRunStatus::Failed;

    EPathFollowingStatus::Type MoveStatus = Controller.GetMoveStatus();
    Controller.MoveToLocation(InstanceData.SoundLocation, AcceptanceRadius);

    if (MoveStatus == EPathFollowingStatus::Idle)
    {
        return EStateTreeRunStatus::Failed; // → Investigate
    }

    return EStateTreeRunStatus::Running;
}