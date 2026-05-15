#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interface/Interact_Interface.h"
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
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereCollisionComp;
	
};
