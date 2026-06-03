#include "InteractPawnBase.h"

#include "Character/Player/HT_Player.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


class AHT_Player;

AInteractPawnBase::AInteractPawnBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComp"));
	SetRootComponent(SphereCollisionComp);
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(SphereCollisionComp);
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);
	
	SphereCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractPawnBase::OnComponentBeginOverlap);
	SphereCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AInteractPawnBase::OnComponentEndOverlap);
}

void AInteractPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractPawnBase::Interacted_Implementation()
{
	if (CanInteract&&CanInteractableObject)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,TEXT("Interacted"));	
	}
}

void AInteractPawnBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanInteractableObject) return;
	
	if (AHT_Player* Player = Cast<AHT_Player>(OtherActor))
	{
		// 오버랩한 플레이어가 로컬 플레이어인지 확인
		if (Player->IsLocallyControlled())
		{
			CanInteract = true;

			CachedPlayer = Player;

			if (ItemOverlayMaterial && SkeletalMeshComp)
			{
				SkeletalMeshComp->SetOverlayMaterial(ItemOverlayMaterial);
			}
			if (WidgetComp)
			{
				WidgetComp->SetVisibility(true);
			}
		}
	}
}

void AInteractPawnBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!CanInteractableObject) return;
	
	if (AHT_Player* Player = Cast<AHT_Player>(OtherActor))
	{
		// 오버랩한 플레이어가 로컬 플레이어인지 확인
		if (Player->IsLocallyControlled())
		{
			CanInteract = false;

			CachedPlayer = nullptr;

			if (ItemOverlayMaterial && SkeletalMeshComp)
			{
				SkeletalMeshComp->SetOverlayMaterial(nullptr);
			}
			if (WidgetComp)
			{
				WidgetComp->SetVisibility(false);
			}
		}
	}
}

