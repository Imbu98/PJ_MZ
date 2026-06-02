#include "EnemyAIControllerBase.h"

#include "EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"

AEnemyAIControllerBase::AEnemyAIControllerBase()
{
    PrimaryActorTick.bCanEverTick = false;
    
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = HearingRange;
    HearingConfig->SetMaxAge(HearingMaxAge);
    HearingConfig->DetectionByAffiliation.bDetectEnemies    = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals   = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;

    AIPerception->ConfigureSense(*HearingConfig);
    AIPerception->SetDominantSense(HearingConfig->GetSenseImplementation());
}

void AEnemyAIControllerBase::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIControllerBase::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyAIControllerBase::PictureTaken()
{
    if (UStateTreeAIComponent* STComp =
    this->FindComponentByClass<UStateTreeAIComponent>())
    {
        FStateTreeEvent Event;
        Event.Tag = FGameplayTag::RequestGameplayTag("Enemy.Stunned");
        STComp->SendStateTreeEvent(Event);

        UE_LOG(LogTemp, Warning, TEXT("Enemy02: 스턴 이벤트 발송"));
    }
}



