
#include "Enemy02AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Perception/AISense_Hearing.h"


AEnemy02AIController::AEnemy02AIController()
{
}

void AEnemy02AIController::BeginPlay()
{
	Super::BeginPlay();
	
	// AIPerception->OnTargetPerceptionUpdated.AddDynamic(
	// 	this, &AEnemy02AIController::OnPerceptionUpdated);
	
	if (AIPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPerception 있음"));
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(
			this, &AEnemy02AIController::OnPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPerception 없음"));
	}
}

void AEnemy02AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGenericTeamId(FGenericTeamId(1));
}

void AEnemy02AIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (GetBlackboardComponent())
			{
				GetBlackboardComponent()->SetValueAsVector(BB_SoundLocation, Stimulus.StimulusLocation);
				GetBlackboardComponent()->SetValueAsObject(BB_TargetActor, Actor);
				UE_LOG(LogTemp, Warning, TEXT("소리 감지됨: %s"), *Stimulus.StimulusLocation.ToString());
			}
		}
	}
}


