#include "InteractActorBase.h"

#include "Components/SphereComponent.h"


AInteractActorBase::AInteractActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComp"));
	SetRootComponent(SphereCollisionComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SphereCollisionComp);
}

void AInteractActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractActorBase::Interacted_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,TEXT("Interacted"));
}

