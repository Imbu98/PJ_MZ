#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoomTypeEntry.h"
#include "RoomBase.h"
#include "DoorComponent.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "DungeonGenerator.generated.h"

USTRUCT()
struct FCachedRoomInfo
{
	GENERATED_BODY()

	ERoomType RoomType = ERoomType::Room;
	FVector BoxExtent = FVector::ZeroVector;
	TArray<FTransform> DoorLocalTransforms;
};

UCLASS(ClassGroup=(Dungeon))
class PJ_MZ_API UDungeonGenerator : public UActorComponent
{
	GENERATED_BODY()

public:
	UDungeonGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon")
	float OverlapTolerance = 0.0f;
	bool bHallSpawned = false;

	UFUNCTION(BlueprintCallable, Category="Dungeon")
	void GenerateDungeon(
		TSubclassOf<ARoomBase> StartRoomClass,
		const TArray<FRoomTypeEntry>& RoomTypeTable,
		int32 MinRooms,
		int32 MaxRooms,
		TArray<ARoomBase*>& OutSpawnedRooms);

	UFUNCTION(BlueprintCallable, Category="Dungeon")
	void ClearDungeon(TArray<ARoomBase*>& SpawnedRooms);

private:
	TMap<UClass*, FCachedRoomInfo> RoomInfoCache;

	void BuildRoomInfoCache(
		TSubclassOf<ARoomBase> StartRoomClass,
		const TArray<FRoomTypeEntry>& RoomTypeTable);

	void CacheRoomInfo(TSubclassOf<ARoomBase> RoomClass);

	TSubclassOf<ARoomBase> PickRoomByWeight(
		const TArray<FRoomTypeEntry>& RoomTypeTable) const;

	FTransform CalculateRoomTransform(
		UDoorComponent* FromDoor,
		const FTransform& ToDoorLocal) const;

	bool IsOverlapping(
		const FTransform& NewTransform,
		const FVector& NewExtent,
		const TArray<ARoomBase*>& SpawnedRooms) const;

	ARoomBase* TrySpawnAndAttachRoom(
		UDoorComponent* FromDoor,
		TSubclassOf<ARoomBase> RoomClass,
		const TArray<ARoomBase*>& SpawnedRooms);
};