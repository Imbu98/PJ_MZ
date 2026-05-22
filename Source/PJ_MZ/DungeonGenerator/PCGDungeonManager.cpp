#include "PCGDungeonManager.h"

APCGDungeonManager::APCGDungeonManager()
{
	PrimaryActorTick.bCanEverTick = false;

	Generator = CreateDefaultSubobject<UDungeonGenerator>(
		TEXT("DungeonGenerator"));
	
	
	Spawner = CreateDefaultSubobject<UObjectSpawner>(
		TEXT("ObjectSpawner"));
}

void APCGDungeonManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateDungeon();
}

void APCGDungeonManager::GenerateDungeon()
{
	if (!IsValid(Generator))
	{
		UE_LOG(LogTemp, Error,
			TEXT("[DungeonManager] Generator가 없음!"));
		return;
	}

	Generator->GenerateDungeon(
		StartRoomClass,
		RoomTypeTable,
		MinRooms,
		MaxRooms,
		SpawnedRooms);
	
	SpawnObjects();
}

void APCGDungeonManager::ClearDungeon()
{
	if (!IsValid(Generator)) return;
	Generator->ClearDungeon(SpawnedRooms);
}

void APCGDungeonManager::SpawnObjects()
{
	if (!IsValid(Spawner))
	{
		UE_LOG(LogTemp, Error,
			TEXT("[DungeonManager] Spawner가 없음!"));
		return;
	}
	
	
	
	Spawner->SpawnObjects(GetSpawnList(), SpawnedRooms, SpawnedItems);
}

void APCGDungeonManager::ClearObjects()
{
	if (!IsValid(Spawner)) return;
	Spawner->ClearObjects(SpawnedItems);
}

TArray<FPicturableDatas> APCGDungeonManager::GetSpawnList()
{
	TArray<FPicturableDatas> Result;

	if (!DT_SpawnCount || !DT_PicturableNameArray || !DT_PicturableDatas) return Result;

	// 1. 스폰 개수 가져오기
	FSpawnCountData* SpawnCount = DT_SpawnCount->FindRow<FSpawnCountData>(FName("Default"), TEXT(""));
	if (!SpawnCount) return Result;

	// 2. 희귀도별 남은 개수 맵
	TMap<ERarity, int32> RemainingCount =
	{
		{ ERarity::NORMAL,    SpawnCount->NormalSpawnCount    },
		{ ERarity::RARE,      SpawnCount->RareSpawnCount      },
		{ ERarity::EPIC,      SpawnCount->EpicSpawnCount      },
		{ ERarity::LEGENDARY, SpawnCount->LegendarySpawnCount },
	};

	// 3. 총 스폰 개수만큼 루프
	int32 TotalCount = SpawnCount->NormalSpawnCount + SpawnCount->RareSpawnCount
					 + SpawnCount->EpicSpawnCount   + SpawnCount->LegendarySpawnCount;

	while (Result.Num() < TotalCount)
	{
		// 아직 남은 희귀도만 후보로
		TArray<ERarity> AvailableRarities;
		for (auto& Pair : RemainingCount)
		{
			if (Pair.Value > 0)
				AvailableRarities.Add(Pair.Key);
		}

		if (AvailableRarities.Num() == 0) break;

		// 4. 랜덤으로 희귀도 하나 선택
		ERarity SelectedRarity = AvailableRarities[FMath::RandRange(0, AvailableRarities.Num() - 1)];
		RemainingCount[SelectedRarity]--;

		// 5. 해당 희귀도에서 랜덤 데이터 1개 가져오기
		TArray<FPicturableDatas> picked = GetRandomPicturablesByRarity(SelectedRarity, 1);
		if (picked.Num() > 0)
		{
			Result.Add(picked[0]);
		}
	}

	return Result;
}

TArray<FPicturableDatas> APCGDungeonManager::GetRandomPicturablesByRarity(ERarity Rarity, int32 Count)
{
	TArray<FPicturableDatas> Result;

	if (!DT_PicturableNameArray || !DT_PicturableDatas) return Result;

	// 1. 희귀도 이름으로 FPicturableArray 로우 가져오기
	FString RarityString = UEnum::GetValueAsString(Rarity);
	RarityString.Split(TEXT("::"), nullptr, &RarityString);

	FPicturableArray* NameArrayRow = DT_PicturableNameArray->FindRow<FPicturableArray>(
		FName(*RarityString), TEXT(""));
	if (!NameArrayRow) return Result;

	// 2. 중복 없이 랜덤 FName 뽑기
	TArray<FName> AvailableNames = NameArrayRow->PicturableRowNames;
	Count = FMath::Min(Count, AvailableNames.Num());

	while (Result.Num() < Count && AvailableNames.Num() > 0)
	{
		int32 RandIndex = FMath::RandRange(0, AvailableNames.Num() - 1);
		FName SelectedName = AvailableNames[RandIndex];
		AvailableNames.RemoveAtSwap(RandIndex);

		// 3. FName으로 DT_PicturableDatas에서 데이터 가져오기
		FPicturableDatas* Data = DT_PicturableDatas->FindRow<FPicturableDatas>(
			SelectedName, TEXT(""));
		if (Data)
		{
			Result.Add(*Data);
		}
	}

	return Result;
}
