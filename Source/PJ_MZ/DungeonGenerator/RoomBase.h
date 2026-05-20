
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DoorComponent.h"
#include "RoomBase.generated.h"

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Start,
	Corridor,
	Room,
	Hall
};

UCLASS()
class PJ_MZ_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	ARoomBase();

protected:
	virtual void BeginPlay() override;
	
	void CollectDoors();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RoomInfo")
	TObjectPtr<UBoxComponent> RoomBounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RoomInfo")
	ERoomType RoomType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RoomInfo")
	TArray<TObjectPtr<UDoorComponent>> Doors;
};
