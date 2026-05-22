

#include "ObjectSpawner.h"
#include "MZ_Datas.h"
#include "PJ_MZ.h"
#include "RoomBase.h"
#include "Components/PicturableComponent.h"
#include "ETC/Picturable/PicturableBase.h"


UObjectSpawner::UObjectSpawner()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectSpawner::SpawnObjects(const TArray<FPicturableDatas> picturableDatas, const TArray<ARoomBase*>& SpawnedRooms, TArray<AActor*>& SpawnedItems)
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
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < picturableDatas.Num(); ++i)
	{
		bool bFound = false; // ← 루프 안으로 이동
		FVector ObjLocation;

		for (int32 Retry = 0; Retry < MaxPlacementRetries; Retry++)
		{
			if (CanSpawnRooms.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("[ObjSpawner] 스폰 가능한 방이 없음!"));
				return;
			}

			ARoomBase* Room = PickRandomRoom(CanSpawnRooms);
			if (!Room) continue;

			if (!FindRandomLocationInRoom(Room, ObjLocation))
			{
				UE_LOG(LogTemp, Warning, TEXT("[ObjSpawner] Retry %d: FindRandomLocation 실패 - Room: %s"),
					Retry, *Room->GetName());
				CanSpawnRooms.Remove(Room);
				continue;
			}

			if (IsTooCloseToOtherItems(ObjLocation, SpawnedItemLocations))
			{
				UE_LOG(LogTemp, Warning, TEXT("[ObjSpawner] Retry %d: TooClose 실패 - Location: %s"),
					Retry, *ObjLocation.ToString());
				continue;
			}

			bFound = true;
			break;
		}

		if (!bFound)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ObjSpawner] 위치 찾기 실패. 스킵."));
			continue;
		}

		APicturableBase* spawnedPicturable = GetWorld()->SpawnActor<APicturableBase>(
			SpawnClass, ObjLocation, FRotator::ZeroRotator, SpawnParams);
		
		if (spawnedPicturable)
		{
			UPicturableComponent* PicturableComp =  spawnedPicturable->FindComponentByClass<UPicturableComponent>();
			if (PicturableComp)
			{
				PicturableComp->SetInfo(picturableDatas[i]);
				
				if (IsValid(spawnedPicturable))
				{
					SpawnedItems.Add(spawnedPicturable);
					SpawnedItemLocations.Add(ObjLocation);

					UE_LOG(LogPJ_MZ, Log,
						TEXT("[ObjSpawner] 아이템 스폰 (%s)"),
						*PicturableComp->PicturableDatas.PicturableName.ToString());
				}
			}
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
{
	if (SpawnedRooms.IsEmpty())
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
	FVector BoxWorldCenter = Room->RoomBounds->GetComponentLocation();
	float Margin = 100.0f;

	float RandX = FMath::RandRange(-Extent.X + Margin, Extent.X - Margin);
	float RandY = FMath::RandRange(-Extent.Y + Margin, Extent.Y - Margin);

	FVector WorldPosition = FVector(
		BoxWorldCenter.X + RandX,
		BoxWorldCenter.Y + RandY,
		BoxWorldCenter.Z
	);

	FVector TraceStart = FVector(WorldPosition.X, WorldPosition.Y, BoxWorldCenter.Z + Extent.Z);
	FVector TraceEnd   = FVector(WorldPosition.X, WorldPosition.Y, BoxWorldCenter.Z - Extent.Z);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 10.0f, 0, 3.0f);
	DrawDebugSphere(GetWorld(), BoxWorldCenter, 50.f, 8, FColor::Green, false, 10.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	// AddIgnoredActor 제거하고 테스트
	// QueryParams.AddIgnoredActor(Room);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams);

	UE_LOG(LogTemp, Warning, TEXT("[Trace] bHit: %s | HitActor: %s | ImpactPoint: %s"),
		bHit ? TEXT("true") : TEXT("false"),
		HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None"),
		*HitResult.ImpactPoint.ToString());

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

