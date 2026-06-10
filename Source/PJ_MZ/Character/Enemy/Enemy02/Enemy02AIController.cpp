
#include "Enemy02AIController.h"

#include "StateTreeEvents.h"
#include "Character/Player/HT_Player.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"


AEnemy02AIController::AEnemy02AIController()
{
	HearingConfig->SetMaxAge(0.1f);
}


void AEnemy02AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy02AIController::OnPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPerception NULL"));
	}
	
	if (InPawn)
	{
		if (UStateTreeAIComponent* STComp = FindComponentByClass<UStateTreeAIComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("StateTreeAIComponent 찾음, StartLogic"));
			STComp->StartLogic();
		}
	}
}

void AEnemy02AIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>()
		&& Stimulus.WasSuccessfullySensed())
	{
		if (UStateTreeAIComponent* STComp =
			this->FindComponentByClass<UStateTreeAIComponent>())
		{
			FStateTreeEvent Event;
			Event.Tag = FGameplayTag::RequestGameplayTag("Enemy.HearSound");
			STComp->SendStateTreeEvent(Event);


			if (Actor)
			{
				AHT_Player* player = Cast<AHT_Player>(Actor);
				if (player)
				{
					const FGameplayTag TutorialTag =FGameplayTag::RequestGameplayTag(TEXT("Tutorial.Enemy02"));
					
					player->AddShownTutorial(TutorialTag);
				}
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Enemy02: 소리 이벤트 발송"));
		}
	}
}


