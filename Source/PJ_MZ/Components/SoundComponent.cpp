#include "SoundComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"


USoundComponent::USoundComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USoundComponent::BeginPlay()
{
	Super::BeginPlay();
	
	owner = GetOwner();
	
}

void USoundComponent::PlayCameraShutterSound()
{
	if (owner&&CameraShutterSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),CameraShutterSound,owner->GetActorLocation());
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), owner->GetActorLocation(), 1.f, owner, 0.f, TEXT("Sound"));
	}
}

void USoundComponent::PlayHorrorBGM()
{
	if (HorrorBGM)
	{
		UGameplayStatics::PlaySound2D(GetWorld(),HorrorBGM,0.5f);
	}
}

