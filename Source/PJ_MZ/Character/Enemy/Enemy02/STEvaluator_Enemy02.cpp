
#include "STEvaluator_Enemy02.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeLinker.h"
#include "Components/StateTreeAIComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"

bool FSTEvaluator_Enemy02::Link(FStateTreeLinker& Linker)
{
	Linker.LinkExternalData(AIControllerHandle);
	Linker.LinkExternalData(PawnHandle);
	return true;
}

void FSTEvaluator_Enemy02::TreeStart(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	InstanceData.AIController = &Context.GetExternalData(AIControllerHandle);
	InstanceData.Pawn = &Context.GetExternalData(PawnHandle);
}

void FSTEvaluator_Enemy02::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	AAIController* Controller = InstanceData.AIController.Get();
	if (!Controller) return;

	APawn* Pawn = InstanceData.Pawn.Get();
	if (!Pawn) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Pawn->GetWorld(), 0);
	if (Player)
	{
		float Distance = FVector::Dist(
			Pawn->GetActorLocation(), Player->GetActorLocation());

		if (Distance <= AttackRange)
		{
			if (UStateTreeAIComponent* STComp =
				Controller->FindComponentByClass<UStateTreeAIComponent>())
			{
				FStateTreeEvent Event;
				Event.Tag = FGameplayTag::RequestGameplayTag("Enemy.Attack");
				STComp->SendStateTreeEvent(Event);
			}
			return;
		}
	}

	UAIPerceptionComponent* Perception = Controller->GetAIPerceptionComponent();
	if (!Perception) return;

	TArray<AActor*> PerceivedActors;
	Perception->GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num() == 0) return;

	FActorPerceptionBlueprintInfo PerceptionInfo;
	if (!Perception->GetActorsPerception(PerceivedActors[0], PerceptionInfo)) return;

	for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>()
			&& Stimulus.WasSuccessfullySensed())
		{
			InstanceData.SoundLocation = Stimulus.StimulusLocation;
			return;
		}
	}
}
