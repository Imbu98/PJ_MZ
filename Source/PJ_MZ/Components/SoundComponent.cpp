#include "SoundComponent.h"

#include "Kismet/GameplayStatics.h"


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
	}
}

