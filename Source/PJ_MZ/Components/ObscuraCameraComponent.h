// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJ_MZ_Delegates.h"
#include "Components/ActorComponent.h"
#include "ObscuraCameraComponent.generated.h"

UENUM(BlueprintType)
enum class EObscuraModeAction : uint8
{
	IDLE,
	CAMERAMODE,
	SHOTTING,
	
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PJ_MZ_API UObscuraCameraComponent   : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObscuraCameraComponent();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	
public:

	// 위젯에서 포인트별 결과를 전달받아 저장
	UFUNCTION(BlueprintCallable)
	void SetPointActive(int32 Index, bool bActive, AActor* HitActor);

	// 활성화된 포인트 수 반환
	UFUNCTION(BlueprintCallable)
	int32 GetActivePointCount() const { return ActivePointCount; }

	// 포인트 수 기반 데미지 배율 반환
	UFUNCTION(BlueprintCallable)
	float GetDamageMultiplier() const;

	// 셔터 시 가장 많이 걸린 몹에게 데미지 적용
	UFUNCTION(BlueprintCallable)
	void ApplyShutterDamage();

	// 포인트 배열 초기화 (위젯이 포인트 수 알려줄 때 호출)
	UFUNCTION(BlueprintCallable)
	void InitPoints(int32 PointCount);
	
	UFUNCTION()
	AActor* GetPrimaryTarget() const;
	
	UFUNCTION()
	void SetObscuraMode(EObscuraModeAction mode){ CurrentMode = mode;};
	
	UFUNCTION()
	EObscuraModeAction GetObscuraMode(){ return CurrentMode;};
	
	UFUNCTION()
	void InitShotCount();
	
	UPROPERTY()
	EObscuraModeAction CurrentMode;
	
public:
	bool IsObscraCooltime = false;
	float currentObscuraCooltime=0.f;
	float MaxObscuraCooltime;
	
public:
	UPROPERTY()
	int32 MaxCanShotCount=3;
	
	UPROPERTY()
	int32 CurrentCanShotCount;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	// float BaseDamage = 30.f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	// TSubclassOf<UDamageType> DamageTypeClass;
	

private:
	TArray<bool> PointActiveList;
	TArray<TWeakObjectPtr<AActor>> PointHitActors;
	int32 ActivePointCount = 0;
};
