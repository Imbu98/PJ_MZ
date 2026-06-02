
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
	Linker.LinkExternalData(STComponentHandle);
	return true;
}

void FSTEvaluator_Enemy02::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	AAIController& Controller = Context.GetExternalData(AIControllerHandle);
	APawn& Pawn = Context.GetExternalData(PawnHandle);
	UStateTreeAIComponent& STComp = Context.GetExternalData(STComponentHandle);

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Controller.GetWorld(), 0);
	if (!Player) return;
	
	float Distance = FVector::Dist(Pawn.GetActorLocation(), Player->GetActorLocation());

	if (Distance <= AttackRange)
	{
		FStateTreeEvent Event;
		Event.Tag = FGameplayTag::RequestGameplayTag("Enemy.Attack");
		STComp.SendStateTreeEvent(Event);
		return;
	}

	UAIPerceptionComponent* Perception = Controller.GetAIPerceptionComponent();
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
