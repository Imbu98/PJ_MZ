#pragma once

#include "CoreMinimal.h"
#include "RoomBase.h"
#include "RoomTypeEntry.generated.h"

USTRUCT(BlueprintType)
struct FRoomTypeEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Room")
	ERoomType RoomType = ERoomType::Room;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Room",
		meta=(ClampMin=1))
	int32 Weight = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Room")
	TArray<TSubclassOf<ARoomBase>> RoomClasses;
};