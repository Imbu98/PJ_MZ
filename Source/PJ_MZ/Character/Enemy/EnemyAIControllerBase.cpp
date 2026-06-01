#include "EnemyAIControllerBase.h"

#include "EnemyBase.h"
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
    // UE_LOG(LogTemp, Warning, TEXT("OnPossess 호출됨"));

    if (!BlackboardAsset || !BehaviorTreeAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("BB 또는 BT 에셋 없음"));
        return;
    }

    UBlackboardComponent* BlackboardComp = nullptr;
    bool bBBResult = UseBlackboard(BlackboardAsset, BlackboardComp);
    bool bBTResult = RunBehaviorTree(BehaviorTreeAsset);

    UE_LOG(LogTemp, Warning, TEXT("BB초기화: %s, BT실행: %s"),
        bBBResult ? TEXT("성공") : TEXT("실패"),
        bBTResult ? TEXT("성공") : TEXT("실패"));
    
    SetGenericTeamId(FGenericTeamId(1));
}


