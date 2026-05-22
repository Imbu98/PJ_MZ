// 여기에 수정 필요 부분에서 AActor 부분 던전Manager랑 같이 생성하실 클래스로 바꿔주시고
// 이 스크립트에는 아이템테이블 받은거 spawnObj에 있고 그거 토대로 랜덤하게 골라서 스폰하시면 됩니다
// 랜덤으로 골라주는 함수만 만드시고 SpawnObj에 제가 주석 남긴 부분에 넣으시면 될거같아요!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectSpawner.generated.h"


class ARoomBase;

UCLASS(ClassGroup=(Dungeon), meta=(BlueprintSpawnableComponent))
class PJ_MZ_API UObjectSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectSpawner();
	
	//(아이템 총 스폰 개수) HT
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectSpawner",
	// 	meta=(ClampMin=0))
	// int32 TotalItemCount = 5;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class APicturableBase> SpawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectSpawner",
		meta=(ClampMin=1))
	int32 MaxPlacementRetries = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectSpawner")
	float MinDistanceBetweenItems = 1000.0f;
	
	//수정필요 HT
	UFUNCTION(BlueprintCallable, Category="ObjectSpawner")
	void SpawnObjects(
		const TArray<FPicturableDatas> picturableDatas,
		const TArray<ARoomBase*>& SpawnedRooms, 
		TArray<AActor*>& SpawnedItems);
	
	UFUNCTION(BlueprintCallable, Category="ObjectSpawner")
	static void ClearObjects(TArray<AActor*>& OutSpawnedItems);

private:
	
	static TArray<ARoomBase*> FilterSpawnableRooms(
		const TArray<ARoomBase*>& SpawnedRooms);
	
	static ARoomBase* PickRandomRoom(
		const TArray<ARoomBase*>& CanSpawnRooms);

	bool FindRandomLocationInRoom(
		ARoomBase* Room,
		FVector& OutLocation) const;
	
	bool IsTooCloseToOtherItems(
		const FVector& Location,
		const TArray<FVector>& SpawnedItemLocations) const;
};

