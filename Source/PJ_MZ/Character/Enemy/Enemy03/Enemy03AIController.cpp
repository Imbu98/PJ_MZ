#include "Enemy03AIController.h"

#include "Components/StateTreeAIComponent.h"
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

    if (InPawn)
    {
        if (UStateTreeAIComponent* STComp = FindComponentByClass<UStateTreeAIComponent>())
        {
            UE_LOG(LogTemp, Warning, TEXT("StateTreeAIComponent 찾음, StartLogic"));
            STComp->StartLogic();
        }
    }
}