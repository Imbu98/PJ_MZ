#include "DungeonGenerator.h"
#include "RoomTypeEntry.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "Engine/World.h"

UDungeonGenerator::UDungeonGenerator()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDungeonGenerator::GenerateDungeon(
    TSubclassOf<ARoomBase> StartRoomClass,
    const TArray<FRoomTypeEntry>& RoomTypeTable,
    int32 MinRooms,
    int32 MaxRooms,
    TArray<ARoomBase*>& OutSpawnedRooms)
{
    OutSpawnedRooms.Empty();

    if (!StartRoomClass)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[Generator] StartRoomClass가 없음!"));
        return;
    }

    BuildRoomInfoCache(StartRoomClass, RoomTypeTable);

    int32 TargetCount = FMath::RandRange(MinRooms, MaxRooms);
    UE_LOG(LogTemp, Log,
        TEXT("[Generator] 목표 방 수: %d"), TargetCount);

    // StartRoom 스폰 및 스폰 옵션 설정
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //왜 항상 스폰으로 설정하냐면 이미 코드에서 정보를 읽어와서 충돌계산 후 스폰하기 떄문에
    //언리얼이 계산해줄 필요도 없을 뿐더러 스폰이 안되는 상황 방지하기 위해서

    ARoomBase* StartRoom = GetWorld()->SpawnActor<ARoomBase>(
        StartRoomClass, FTransform::Identity, SpawnParams);

    if (!StartRoom)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[Generator] StartRoom 스폰 실패!"));
        return;
    }

    OutSpawnedRooms.Add(StartRoom);

    // 처리할 문 목록 (어떤 방의 어떤 문인지)
    // 미로처럼 만들기 위해 모든 열린 문을 큐에 넣고 처리
    TArray<TPair<ARoomBase*, UDoorComponent*>> OpenDoors;

    // StartRoom의 모든 문을 큐에 추가
    for (UDoorComponent* Door : StartRoom->Doors)
    {
        if (Door && Door->IsAvailable())
        {
            OpenDoors.Add(TPair<ARoomBase*, UDoorComponent*>(
                StartRoom, Door));
        }
    }
    
    // 열린 문이 있고 목표 방 수에 안 달했으면 계속
    while (!OpenDoors.IsEmpty() &&
           OutSpawnedRooms.Num() < TargetCount)
    {
        // 랜덤으로 열린 문 하나 선택 (미로처럼 퍼지게)
        int32 RandDoorIdx = FMath::RandRange(0, OpenDoors.Num() - 1);
        TPair<ARoomBase*, UDoorComponent*> DoorPair =
            OpenDoors[RandDoorIdx];
        OpenDoors.RemoveAt(RandDoorIdx);

        UDoorComponent* FromDoor = DoorPair.Value;

        // 이미 사용된 문이면 스킵
        if (!FromDoor || !FromDoor->IsAvailable()) continue;
        
        TSubclassOf<ARoomBase> NextRoomClass;
        
        if (!bHallSpawned && OutSpawnedRooms.Num() > TargetCount/2)
        {
            ARoomBase** FoundRoom = OutSpawnedRooms.FindByPredicate([](ARoomBase* Room)
                {return Room && Room->RoomType == ERoomType::Hall;});
        
            if (!FoundRoom)
            {
                TSubclassOf<ARoomBase> HallClass = nullptr;
        
                for (const FRoomTypeEntry& Entry : RoomTypeTable)
                {
                    if (Entry.RoomType == ERoomType::Hall &&
                        !Entry.RoomClasses.IsEmpty())
                    {
                        int32 Idx = FMath::RandRange(0, Entry.RoomClasses.Num() - 1);
                        HallClass = Entry.RoomClasses[Idx];
                        break;
                    }
                }
                NextRoomClass = HallClass;
                UE_LOG(LogTemp, Log,
                    TEXT("[Generator] Hall 생성시도"),
                    OutSpawnedRooms.Num());
            }
            
            bHallSpawned = true;
        }
        else
        {
            NextRoomClass = PickRoomByWeight(RoomTypeTable);
        }
        

        if (!NextRoomClass) continue;

        ARoomBase* NewRoom = TrySpawnAndAttachRoom(
            FromDoor, NextRoomClass, OutSpawnedRooms);
        
        if (NewRoom)
        {
            FromDoor->bUsed = true;
            OutSpawnedRooms.Add(NewRoom);

            // 새 방의 모든 열린 문을 큐에 추가
            for (UDoorComponent* Door : NewRoom->Doors)
            {
                if (Door && Door->IsAvailable())
                {
                    OpenDoors.Add(
                        TPair<ARoomBase*, UDoorComponent*>(
                            NewRoom, Door));
                }
            }

            UE_LOG(LogTemp, Log,
                TEXT("[Generator] 방 추가 (%d/%d): %s"),
                OutSpawnedRooms.Num(), TargetCount,
                *NextRoomClass->GetName());
        }
        else
        {
            FromDoor->bBlocked = true;

            UE_LOG(LogTemp, Warning,
                TEXT("[Generator] 문 막힘: %s"),
                *FromDoor->GetName());
        }
    }

    UE_LOG(LogTemp, Log,
        TEXT("[Generator] 던전 생성 완료. 총 %d개 방"),
        OutSpawnedRooms.Num());
    
    bGenerationComplete = true;
    OnDungeonGenerationComplete.Broadcast();
}

void UDungeonGenerator::ClearDungeon(TArray<ARoomBase*>& SpawnedRooms)
{
    for (ARoomBase* Room : SpawnedRooms)
    {
        if (IsValid(Room))
        {
            Room->Destroy();
        }
    }
    SpawnedRooms.Empty();
    RoomInfoCache.Empty();
}

void UDungeonGenerator::BuildRoomInfoCache(
    TSubclassOf<ARoomBase> StartRoomClass,
    const TArray<FRoomTypeEntry>& RoomTypeTable)
{
    RoomInfoCache.Empty();

    if (StartRoomClass)
    {
        CacheRoomInfo(StartRoomClass);
    }

    for (const FRoomTypeEntry& Entry : RoomTypeTable)
    {
        for (TSubclassOf<ARoomBase> RoomClass : Entry.RoomClasses)
        {
            if (RoomClass)
            {
                CacheRoomInfo(RoomClass);
            }
        }
    }

    UE_LOG(LogTemp, Log,
        TEXT("[Generator] 캐시 완료: %d개 방"),
        RoomInfoCache.Num());
}

void UDungeonGenerator::CacheRoomInfo(TSubclassOf<ARoomBase> RoomClass)
{
    if (!RoomClass) return;
    if (RoomInfoCache.Contains(RoomClass)) return;

    FCachedRoomInfo Info;

    // CDO에서 RoomType, BoxExtent 읽기
    if (const ARoomBase* CDO = GetDefault<ARoomBase>(RoomClass))
    {
        Info.RoomType = CDO->RoomType;

        if (CDO->RoomBounds)
        {
            Info.BoxExtent =
                CDO->RoomBounds->GetUnscaledBoxExtent();
        }
    }

    // SimpleConstructionScript로 DoorComponent 읽기
    if (UBlueprintGeneratedClass* BPGC =
        Cast<UBlueprintGeneratedClass>(RoomClass))
    {
        if (BPGC->SimpleConstructionScript)
        {
            for (USCS_Node* Node :
                BPGC->SimpleConstructionScript->GetAllNodes())
            {
                if (!Node) continue;

                UActorComponent* CompTemplate =
                    Node->GetActualComponentTemplate(BPGC);

                if (UDoorComponent* DoorTemplate =
                    Cast<UDoorComponent>(CompTemplate))
                {
                    Info.DoorLocalTransforms.Add(
                        DoorTemplate->GetRelativeTransform());
                }
            }
        }
    }

    UE_LOG(LogTemp, Log,
        TEXT("[Generator] 캐시: %s | 문 %d개 | Extent %s"),
        *RoomClass->GetName(),
        Info.DoorLocalTransforms.Num(),
        *Info.BoxExtent.ToString());

    RoomInfoCache.Add(RoomClass, Info);
}

TSubclassOf<ARoomBase> UDungeonGenerator::PickRoomByWeight(
    const TArray<FRoomTypeEntry>& RoomTypeTable) const
{
    if (RoomTypeTable.IsEmpty()) return nullptr;

    int32 TotalWeight = 0;
    for (const FRoomTypeEntry& Entry : RoomTypeTable)
    {
        if (!Entry.RoomClasses.IsEmpty())
        {
            TotalWeight += Entry.Weight;
        }
    }

    if (TotalWeight <= 0) return nullptr;

    int32 RandValue = FMath::RandRange(0, TotalWeight - 1);
    int32 Accumulated = 0;

    for (const FRoomTypeEntry& Entry : RoomTypeTable)
    {
        if (Entry.RoomClasses.IsEmpty()) continue;

        Accumulated += Entry.Weight;
        
        if (RandValue < Accumulated)
        {
            int32 Idx = FMath::RandRange(
                0, Entry.RoomClasses.Num() - 1);
            return Entry.RoomClasses[Idx];
        }
    }

    return nullptr;
}

FTransform UDungeonGenerator::CalculateRoomTransform(
    UDoorComponent* FromDoor,
    const FTransform& ToDoorLocal) const
{
    // FromDoor 월드 트랜스폼 가져오기
    FTransform FromDoorWorld = FromDoor->GetComponentTransform();

    // Yaw 180도 회전 (문이 서로 마주보도록)
    FRotator FlippedRot = FromDoorWorld.Rotator();
    FlippedRot.Yaw += 180.0f;
    FromDoorWorld.SetRotation(FlippedRot.Quaternion());

    // 새 방 트랜스폼 계산
    // ToDoorLocal.Inverse() * FlippedFromDoor
    return ToDoorLocal.Inverse() * FromDoorWorld;
}

bool UDungeonGenerator::IsOverlapping(
    const FTransform& NewTransform,
    const FVector& NewExtent,
    const TArray<ARoomBase*>& SpawnedRooms) const
{
    FVector NewCenter = NewTransform.GetLocation();

    for (const ARoomBase* ExistingRoom : SpawnedRooms)
    {
        if (!IsValid(ExistingRoom)) continue;
        if (!ExistingRoom->RoomBounds) continue;

        FVector ExistingCenter = ExistingRoom->GetActorLocation();
        FVector ExistingExtent =
            ExistingRoom->RoomBounds->GetScaledBoxExtent();

        FVector Distance = (NewCenter - ExistingCenter).GetAbs();
        
        // 딱 붙어있는 건 허용, 실제로 침범할 때만 겹침으로 판단
        // 1.0f 여유를 줘서 부동소수점 오차 처리
        FVector SumExtent = NewExtent + ExistingExtent - FVector(1.0f);

        if (Distance.X < SumExtent.X &&
            Distance.Y < SumExtent.Y &&
            Distance.Z < SumExtent.Z)
        {
            return true;
        }
    }

    return false;
}

ARoomBase* UDungeonGenerator::TrySpawnAndAttachRoom(
    UDoorComponent* FromDoor,
    TSubclassOf<ARoomBase> RoomClass,
    const TArray<ARoomBase*>& SpawnedRooms)
{
    if (!FromDoor || !RoomClass) return nullptr;

    FCachedRoomInfo* CachedInfo = RoomInfoCache.Find(RoomClass);
    if (!CachedInfo)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[Generator] 캐시 없음: %s"),
            *RoomClass->GetName());
        return nullptr;
    }

    if (CachedInfo->DoorLocalTransforms.IsEmpty())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[Generator] 문 정보 없음: %s"),
            *RoomClass->GetName());
        return nullptr;
    }

    // 문 인덱스 셔플
    TArray<int32> DoorIndices;
    for (int32 i = 0; i < CachedInfo->DoorLocalTransforms.Num(); i++)
    {
        DoorIndices.Add(i);
    }

    for (int32 i = DoorIndices.Num() - 1; i > 0; i--)
    {
        int32 j = FMath::RandRange(0, i);
        DoorIndices.Swap(i, j);
    }

    // 각 문마다 겹침 체크 후 스폰 시도
    for (int32 DoorIdx : DoorIndices)
    {
        const FTransform& ToDoorLocal =
            CachedInfo->DoorLocalTransforms[DoorIdx];

        FTransform NewRoomTransform =
            CalculateRoomTransform(FromDoor, ToDoorLocal);
        
        UE_LOG(LogTemp, Log,
        TEXT("[Generator] 새 방 위치: %s"),
        *NewRoomTransform.GetLocation().ToString());

        if (IsOverlapping(
            NewRoomTransform, CachedInfo->BoxExtent, SpawnedRooms))
        {
            UE_LOG(LogTemp, Warning,
    TEXT("[Generator] 겹침! 위치: %s"),
    *NewRoomTransform.GetLocation().ToString());
            continue;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        ARoomBase* NewRoom = GetWorld()->SpawnActor<ARoomBase>(
            RoomClass,
            NewRoomTransform,
            SpawnParams);

        if (IsValid(NewRoom))
        {
            // 연결된 문 Used 처리
            if (NewRoom->Doors.IsValidIndex(DoorIdx))
            {
                NewRoom->Doors[DoorIdx]->bUsed = true;
            }

            return NewRoom;
        }
    }

    return nullptr;
}