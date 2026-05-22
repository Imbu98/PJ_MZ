#include "EnemyAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
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

    FString BBPath = GetBlackboardPath();
    FString BTPath = GetBehaviorTreePath();

    if (BBPath.IsEmpty() || BTPath.IsEmpty()) return;

    UBlackboardData* BBAsset = LoadObject<UBlackboardData>(nullptr, *BBPath);
    UBehaviorTree* BTAsset   = LoadObject<UBehaviorTree>(nullptr, *BTPath);

    if (BBAsset && BTAsset)
    {
        UBlackboardComponent* BlackboardComp = nullptr;
        UseBlackboard(BBAsset, BlackboardComp);
        RunBehaviorTree(BTAsset);
    }
}


