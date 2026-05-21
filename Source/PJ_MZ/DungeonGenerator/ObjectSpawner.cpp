

#include "ObjectSpawner.h"

#include "ImathMath.h"
#include "ImathMath.h"
#include "ImathMath.h"
#include "RoomBase.h"
#include "AI/Navigation/NavigationDataResolution.h"
#include "Character/Enemy/EnemyBase.h"
#include "Settings/HomeScreenCommon.h"


UObjectSpawner::UObjectSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectSpawner::SpawnObjects(
	const TArray<AActor*> ItemTypeTable, const TArray<ARoomBase*>& SpawnedRooms, TArray<AActor*>& SpawnedItems)
{
	TArray<ARoomBase*> CanSpawnRooms = FilterSpawnableRooms(SpawnedRooms);
	
	if (CanSpawnRooms.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ObjSpawner] 스폰 가능한 방이 없음!"));
		return;
	}
	
	TArray<FVector> SpawnedItemLocations;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector ObjLocation;
		bool bFound = false;

	for (int32 i = 0; i < TotalItemCount; ++i)
	{
		
		//랜덤 아이템 뽑아주는 함수 호출
		
		for (int32 Retry = 0; Retry < MaxPlacementRetries; Retry++)
		{
			if (CanSpawnRooms.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("[ObjSpawner] 스폰 가능한 방이 없음!"));
				return;
			}
			
			if (ARoomBase* Room = PickRandomRoom(CanSpawnRooms))
			{
				if (!FindRandomLocationInRoom(Room, ObjLocation))
				{
					CanSpawnRooms.Remove(Room);
					continue;
				}

				if (IsTooCloseToOtherItems(ObjLocation, SpawnedItemLocations))
				{
					CanSpawnRooms.Remove(Room);
					continue;
				}

				bFound = true;
				break;
			}
		}
		
		if (!bFound)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[ObjSpawner] 위치 찾기 실패. 스킵."));
			continue;
		}
		
		AActor* SpawnObj = GetWorld()->SpawnActor<AActor>(
			////여기에 랜덤 오브젝트 class 뽑아서 넣기,
			ObjLocation,
			FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f),
			SpawnParams);
		
		if (IsValid(SpawnObj))
		{
			SpawnedItems.Add(SpawnObj);
			SpawnedItemLocations.Add(ObjLocation);

			UE_LOG(LogTemp, Log,
				TEXT("[ObjSpawner] 아이템 스폰 (%d/%d)"),
				SpawnedItems.Num(), TotalItemCount);
		}
		
	}
	UE_LOG(LogTemp, Log,
		TEXT("[ObjSpawner] 스폰 완료. 총 %d개"),
		SpawnedItems.Num());
}

void UObjectSpawner::ClearObjects(TArray<AActor*>& OutSpawnedItems)
{
	for (AActor* Item : OutSpawnedItems)
	{
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
	OutSpawnedItems.Empty();
}

TArray<ARoomBase*> UObjectSpawner::FilterSpawnableRooms(const TArray<ARoomBase*>& SpawnedRooms)
{;
	if (!SpawnedRooms.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("[ObjSpawner] 던전 방이 존재하지 않습니다."));
		return {};
	}
	
	return SpawnedRooms.FilterByPredicate([](const ARoomBase* Room)
	{return IsValid(Room) && Room->bCanSpawnObj;});
}

ARoomBase* UObjectSpawner::PickRandomRoom(const TArray<ARoomBase*>& CanSpawnRooms)
{
	if (CanSpawnRooms.IsEmpty()) return nullptr;
	
	return CanSpawnRooms[FMath::RandRange(0, CanSpawnRooms.Num() - 1)];
}


bool UObjectSpawner::FindRandomLocationInRoom(ARoomBase* Room, FVector& OutLocation) const
{
	if (!IsValid(Room) || !Room->RoomBounds) return false;

	FVector Extent = Room->RoomBounds->GetScaledBoxExtent();
	float Margin = 100.0f;

	float RandX = FMath::RandRange(-Extent.X + Margin, Extent.X - Margin);
	float RandY = FMath::RandRange(-Extent.Y + Margin, Extent.Y - Margin);

	FVector LocalOffset = FVector(RandX, RandY, 0.0f);

	FVector WorldPosition = Room->GetActorTransform()
		.TransformPosition(LocalOffset);

	FVector TraceStart = WorldPosition + FVector(0, 0, Extent.Z);
	FVector TraceEnd   = WorldPosition - FVector(0, 0, Extent.Z);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Room);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams);

	if (!bHit) return false;

	OutLocation = HitResult.ImpactPoint + FVector(0, 0, 10.0f);
	return true;
}

bool UObjectSpawner::IsTooCloseToOtherItems(const FVector& Location, const TArray<FVector>& SpawnedItemLocations) const
{
	for (const FVector& ExistingLocation : SpawnedItemLocations)
	{
		if (FVector::Dist(Location, ExistingLocation) < MinDistanceBetweenItems)
		{
			return true;
		}
	}
	return false;
}

