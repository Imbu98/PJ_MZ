#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypeEntry.h"
#include "DungeonGenerator.h"
#include "RoomBase.h"
#include "PCGDungeonManager.generated.h"

UCLASS()
class PJ_MZ_API APCGDungeonManager : public AActor
{
    GENERATED_BODY()

public:
    APCGDungeonManager();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup")
    TSubclassOf<ARoomBase> StartRoomClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup")
    TArray<FRoomTypeEntry> RoomTypeTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup",
        meta=(ClampMin=1))
    int32 MinRooms = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup",
        meta=(ClampMin=1))
    int32 MaxRooms = 12;

    UPROPERTY(BlueprintReadOnly, Category="Dungeon|Runtime")
    TArray<ARoomBase*> SpawnedRooms;

    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void GenerateDungeon();

    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void ClearDungeon();

private:
    UPROPERTY()
    TObjectPtr<UDungeonGenerator> Generator;
};