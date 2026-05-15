
#include "PCGManager.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"


APCGManager::APCGManager()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CurrentRoom = nullptr;
}

void APCGManager::BeginPlay()
{
	Super::BeginPlay();

	StartGenerate();
}

void APCGManager::StartGenerate()
{
	SpawnRoom();
}

void APCGManager::SpawnRoom()
{
	if (!RoomClass) return;

	ARoomBase* NewRoom =
		GetWorld()->SpawnActor<ARoomBase>(RoomClass);

	if (CurrentRoom)
	{
		ConnectRooms(CurrentRoom, NewRoom);
	}

	CurrentRoom = NewRoom;
}

void APCGManager::ConnectRooms(ARoomBase* A, ARoomBase* B)
{
	if (!A || !B) return;

	for (FDoorData& DoorA : A->Doors)
	{
		for (FDoorData& DoorB : B->Doors)
		{
			if (DoorA.bUsed || DoorB.bUsed)
				continue;

			FVector DirA = DoorA.DoorArrow->GetForwardVector();
			FVector DirB = DoorB.DoorArrow->GetForwardVector();

			// 서로 반대 방향일 때만 연결
			if (FVector::DotProduct(DirA, DirB) < -0.9f)
			{
				DoorA.bUsed = true;
				DoorB.bUsed = true;

				FVector TargetLocation =
					DoorA.DoorArrow->GetComponentLocation()
					- DoorB.DoorArrow->GetForwardVector() * 100.f;

				B->SetActorLocation(TargetLocation);

				return;
			}
		}
	}
}

