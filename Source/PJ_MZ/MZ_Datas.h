#pragma once

#include "CoreMinimal.h"
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
struct FPicturable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
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
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Set2BonusScore = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Set3BonusScore = 0.f;
};

USTRUCT(BlueprintType)
struct FPicturableArray : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	// Row Name = Rarity ("NORMAL", "RARE", "EPIC", "LEGENDARY")
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> PicturableRowNames; // FGuid 대신 Row Name(FName) 사용
};
