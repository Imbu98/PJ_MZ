#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "DoorComponent.generated.h"


UCLASS(ClassGroup=(Dungeon), meta=(BlueprintSpawnableComponent))
class PJ_MZ_API UDoorComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	UDoorComponent();

	UPROPERTY(BlueprintReadOnly, Category="Door")
	bool bUsed = false;
	
	UPROPERTY(BlueprintReadOnly, Category="Door")
	bool bBlocked = false;
	
	UFUNCTION(BlueprintCallable, Category="Door")
	bool IsAvailable() const { return !bUsed && !bBlocked; }
	//
	// UFUNCTION(BlueprintCallable, Category="Door")
	// FVector GetWorldDirection() const { return GetForwardVector(); }
	//
	// UFUNCTION(BlueprintCallable, Category="Door")
	// FTransform GetWorldTransform() const { return GetComponentTransform(); }
	//
};
