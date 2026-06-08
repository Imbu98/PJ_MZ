#include "Enemy02Character.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy02AIController.h"
#include "Components/AudioComponent.h"

AEnemy02Character::AEnemy02Character()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	BreathAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComp"));
}

void AEnemy02Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (BreathAudioComp && SoundAttenuation)
	{
		BreathAudioComp->AttenuationSettings = SoundAttenuation;
	}
	if (BreathAudioComp && BreathSound)
	{
		BreathAudioComp->SetSound(BreathSound);
	}
}




