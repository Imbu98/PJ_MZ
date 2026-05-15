#include "PCGDungeonManager.h"

APCGDungeonManager::APCGDungeonManager()
{
	PrimaryActorTick.bCanEverTick = false;

	Generator = CreateDefaultSubobject<UDungeonGenerator>(
		TEXT("DungeonGenerator"));
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
}

void APCGDungeonManager::ClearDungeon()
{
	if (!IsValid(Generator)) return;
	Generator->ClearDungeon(SpawnedRooms);
}