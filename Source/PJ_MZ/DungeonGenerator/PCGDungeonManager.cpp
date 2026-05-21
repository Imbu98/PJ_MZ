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
	
	Spawner->SpawnObjects(ObjTypeTable, SpawnedRooms, SpawnedItems);
	
}

void APCGDungeonManager::ClearObjects()
{
	if (!IsValid(Spawner)) return;
	Spawner->ClearObjects(SpawnedItems);
}
