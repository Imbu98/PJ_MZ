// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomBase.h"
#include "PCGManager.generated.h"

UCLASS()
class PJ_MZ_API APCGManager : public AActor
{
	GENERATED_BODY()

public:
	APCGManager();

protected:
	virtual void BeginPlay() override;

public:
	// ===== PCG 시작 함수 =====
	void StartGenerate();

	// ===== 방 생성 =====
	void SpawnRoom();

	// ===== 방 연결 =====
	void ConnectRooms(ARoomBase* A, ARoomBase* B);
	
	// ===== 설정값 =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PCG")
	TSubclassOf<ARoomBase> RoomClass;

	// ===== 현재 생성된 방 =====
	UPROPERTY()
	ARoomBase* CurrentRoom;
};
