#include "ObscuraCameraComponent.h"


UObscuraCameraComponent::UObscuraCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UObscuraCameraComponent  ::BeginPlay()
{
	Super::BeginPlay();
	
}

void UObscuraCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

