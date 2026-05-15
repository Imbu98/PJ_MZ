
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "RoomBase.generated.h"

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Room,
	Corridor
};

USTRUCT(BlueprintType)
struct FDoorData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> DoorArrow;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUsed = false;
};

UCLASS()
class PJ_MZ_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	ARoomBase();

protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RoomInfo")
	TObjectPtr<UBoxComponent> RoomBounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RoomInfo")
	ERoomType RoomType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="RoomInfo")
	TArray<FDoorData> Doors;
	
	UFUNCTION()
	void BuildDoors();
};
