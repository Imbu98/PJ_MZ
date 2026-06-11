#include "InteractActorBase.h"

#include "Character/Player/HT_Player.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


AInteractActorBase::AInteractActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComp"));
	SetRootComponent(SphereCollisionComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SphereCollisionComp);
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);
	
	SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractActorBase::OnComponentBeginOverlap);
	SphereCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AInteractActorBase::OnComponentEndOverlap);
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
	if (CanInteract&&CanInteractableObject)
	{
		//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,TEXT("Interacted"));	
	}
}

void AInteractActorBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanInteractableObject) return;
	
	if (AHT_Player* Player = Cast<AHT_Player>(OtherActor))
	{
		// 오버랩한 플레이어가 로컬 플레이어인지 확인
		if (Player->IsLocallyControlled())
		{
			CanInteract = true;

			if (ItemOverlayMaterial && StaticMeshComp)
			{
				StaticMeshComp->SetOverlayMaterial(ItemOverlayMaterial);
			}
			if (WidgetComp)
			{
				WidgetComp->SetVisibility(true);
			}
		}
	}
}

void AInteractActorBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!CanInteractableObject) return;
	
	if (AHT_Player* Player = Cast<AHT_Player>(OtherActor))
	{
		// 오버랩한 플레이어가 로컬 플레이어인지 확인
		if (Player->IsLocallyControlled())
		{
			CanInteract = false;

			if (ItemOverlayMaterial && StaticMeshComp)
			{
				StaticMeshComp->SetOverlayMaterial(nullptr);
			}
			if (WidgetComp)
			{
				WidgetComp->SetVisibility(false);
			}
		}
	}
}


