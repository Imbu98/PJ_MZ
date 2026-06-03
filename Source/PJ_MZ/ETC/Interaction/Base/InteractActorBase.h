#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../Interface/Interact_Interface.h"
#include "InteractActorBase.generated.h"

UCLASS()
class PJ_MZ_API AInteractActorBase : public AActor, public IInteract_Interface 

{
	GENERATED_BODY()

public:
	AInteractActorBase();
	
	virtual void Tick(float DeltaTime) override;
	
	// Interface Function
	virtual void Interacted_Implementation() override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereCollisionComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> WidgetComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UMaterialInterface> ItemOverlayMaterial;
	
	// 현재 상호작용 할 수 있는 상태인지
	bool CanInteract=false;
	
	// 상호작용가능한 물체인지
	bool CanInteractableObject=true;
	
};
