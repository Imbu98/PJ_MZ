#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/Interact_Interface.h"
#include "InteractPawnBase.generated.h"

UCLASS()
class PJ_MZ_API AInteractPawnBase : public APawn , public IInteract_Interface
{
	GENERATED_BODY()

public:
	AInteractPawnBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Interacted_Implementation() override;

protected:

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereCollisionComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> ItemOverlayMaterial;

	UPROPERTY()
	TObjectPtr<class AHT_Player> CachedPlayer;
	
	// 현재 상호작용 할 수 있는 상태인지
	bool CanInteract=false;
	
	// 상호작용가능한 물체인지
	bool CanInteractableObject=true;
};
