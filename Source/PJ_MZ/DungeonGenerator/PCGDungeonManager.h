#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTypeEntry.h"
#include "DungeonGenerator.h"
#include "ObjectSpawner.h"
#include "RoomBase.h"
#include "MZ_Datas.h"
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


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup")
    TArray<AActor*> ObjTypeTable;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup",
        meta=(ClampMin=1))
    int32 MinRooms = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dungeon|Setup",
        meta=(ClampMin=1))
    int32 MaxRooms = 12;

    UPROPERTY(BlueprintReadOnly, Category="Dungeon|Runtime")
    TArray<ARoomBase*> SpawnedRooms;
    
    UPROPERTY(BlueprintReadOnly, Category="Dungeon|Runtime")
    TArray<AActor*> SpawnedItems;

    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void GenerateDungeon();

    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void ClearDungeon();
    
    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void SpawnObjects();

    UFUNCTION(BlueprintCallable, Category="Dungeon")
    void ClearObjects();
    
    
    // ===============================
    // 관련 데이터 테이블
    // ===============================
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TObjectPtr<UDataTable> DT_PicturableNameArray;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TObjectPtr<UDataTable> DT_PicturableDatas;
    
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TObjectPtr<UDataTable> DT_SpawnCount;
    
    UFUNCTION()
    TArray<FPicturableDatas> GetSpawnList();
    
    UFUNCTION()
    TArray<FPicturableDatas> GetRandomPicturablesByRarity(ERarity Rarity, int32 Count);
    
    

private:
    UPROPERTY()
    TObjectPtr<UDungeonGenerator> Generator;
    
public:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TObjectPtr<UObjectSpawner> Spawner;
};