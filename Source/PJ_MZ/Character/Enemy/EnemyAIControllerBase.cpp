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
    UE_LOG(LogTemp, Warning, TEXT("OnPossess 호출됨"));

    FString BBPath = GetBlackboardPath();
    FString BTPath = GetBehaviorTreePath();
    
    UE_LOG(LogTemp, Warning, TEXT("BB경로: %s"), *BBPath);
    UE_LOG(LogTemp, Warning, TEXT("BT경로: %s"), *BTPath);

    if (BBPath.IsEmpty() || BTPath.IsEmpty()) 
    {
        UE_LOG(LogTemp, Warning, TEXT("경로 비어있음"));
        return;
    }
    
    UBlackboardData* BBAsset = LoadObject<UBlackboardData>(nullptr, *BBPath);
    UBehaviorTree* BTAsset   = LoadObject<UBehaviorTree>(nullptr, *BTPath);

    UE_LOG(LogTemp, Warning, TEXT("BB: %s, BT: %s"), 
    BBAsset ? TEXT("로드성공") : TEXT("로드실패"),
    BTAsset ? TEXT("로드성공") : TEXT("로드실패"));
    
    if (BBAsset && BTAsset)
    {
        UBlackboardComponent* BlackboardComp = nullptr;
        bool bBBResult = UseBlackboard(BBAsset, BlackboardComp);
        bool bBTResult = RunBehaviorTree(BTAsset);
    
        UE_LOG(LogTemp, Warning, TEXT("BB초기화: %s, BT실행: %s"),
            bBBResult ? TEXT("성공") : TEXT("실패"),
            bBTResult ? TEXT("성공") : TEXT("실패"));
    }

}


