#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MZ_Datas.generated.h"

UENUM(BlueprintType)
enum class ERarity : uint8
{
	NORMAL,
	RARE,
	EPIC,
	LEGENDARY,
};


USTRUCT(BlueprintType)
struct FPicturableDatas : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName PicturableName;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float PicturableMaxScore = 0.f;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSoftObjectPtr<UStaticMesh> PicturableStaticMesh;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	ERarity PicturableRarity;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName SetID;
};

USTRUCT(BlueprintType)
struct FSetBonus : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Set2BonusScore = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Set3BonusScore = 0.f;
};

USTRUCT(BlueprintType)
struct FPicturableArray : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	// Row Name = Rarity ("NORMAL", "RARE", "EPIC", "LEGENDARY")
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> PicturableRowNames; // FGuid 대신 Row Name(FName) 사용
};

USTRUCT(BlueprintType)
struct FSpawnCountData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NormalSpawnCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RareSpawnCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 EpicSpawnCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 LegendarySpawnCount;
};