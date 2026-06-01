
#include "Enemy02AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"


AEnemy02AIController::AEnemy02AIController()
{
	HearingConfig->SetMaxAge(0.1f);
}

void AEnemy02AIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy02AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (AIPerception)
	{
		// // UE_LOG(LogTemp, Warning, TEXT("AIPerception 유효")); 
		// AAIController* ExistingController = Cast<AAIController>(InPawn->GetController());
		// UE_LOG(LogTemp, Warning, TEXT("기존 컨트롤러: %s"), ExistingController ? *ExistingController->GetName() : TEXT("없음"));
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy02AIController::OnPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPerception NULL"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Perception 바인딩 완료"));
}

void AEnemy02AIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// UE_LOG(LogTemp, Warning, TEXT("OnPerceptionUpdated 호출됨"));
	// UE_LOG(LogTemp, Warning, TEXT("OnPerceptionUpdated 호출됨 - 감지여부: %s"), 
	//   Stimulus.WasSuccessfullySensed() ? TEXT("감지") : TEXT("만료"));
	
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (GetBlackboardComponent())
			{
				GetBlackboardComponent()->SetValueAsVector(BB_SoundLocation, Stimulus.StimulusLocation);
				GetBlackboardComponent()->SetValueAsObject(BB_TargetActor, Actor);
				GetBlackboardComponent()->SetValueAsBool(TEXT("bHearSound"), true);
				
				// UE_LOG(LogTemp, Warning, TEXT("소리 감지됨: %s"), *Stimulus.StimulusLocation.ToString());
			}
		}
	}
}


