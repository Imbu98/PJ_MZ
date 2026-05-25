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
	UPROPERTY()
	EObscuraModeAction CurrentMode;
	
	UPROPERTY()
	TObjectPtr<class AHT_PlayerState> Cached_PS;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	// float BaseDamage = 30.f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	// TSubclassOf<UDamageType> DamageTypeClass;
private:
	TArray<bool> PointActiveList;
	TArray<TWeakObjectPtr<AActor>> PointHitActors;
	int32 ActivePointCount = 0;
	
public:
	// 위젯에서 포인트별 결과를 전달받아 저장
	UFUNCTION(BlueprintCallable)
	void SetPointActive(int32 Index, bool bActive, AActor* HitActor);

	// 활성화된 포인트 수 반환
	UFUNCTION(BlueprintCallable)
	int32 GetActivePointCount() const { return ActivePointCount; }

	// 포인트 수 기반 데미지 배율 반환
	UFUNCTION(BlueprintCallable)
	float GetFinalScore(float score) const;

	// 셔터 시 가장 많이 걸린 몹에게 데미지 적용
	UFUNCTION(BlueprintCallable)
	void ApplyShutterDamage();

	// 포인트 배열 초기화 (위젯이 포인트 수 알려줄 때 호출)
	UFUNCTION(BlueprintCallable)
	void InitPoints(int32 PointCount);
	
	UFUNCTION()
	void SetObscuraMode(EObscuraModeAction mode){ CurrentMode = mode;};
	
	UFUNCTION()
	EObscuraModeAction GetObscuraMode(){ return CurrentMode;};
	
	UFUNCTION()
	bool ObscuraCanShot();
	
	UFUNCTION()
	float GetObscuraCooltimePercent();
	
	// 결과 창 (임시)
	UFUNCTION()
	void SetResultUI();

	UFUNCTION()
	void OnGameEnd();
	
	// 현재 찍은 Picturable오브젝트의 점수를 저장
	UFUNCTION()
	void AddCurrentScoreToArray(float scoreVal);
	
	// 현재 Playerstate의 점수배열의 점수 가져오기
	float GetScoreArrayValue(int32 index);
	
	
	// ===============================
	// 캡쳐 관련
	// ===============================
private:
	UPROPERTY(VisibleAnywhere)
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

public:
	UFUNCTION()
	void CapturePhoto();
	
	// ===============================
	// 사진 액터 저장
	// ===============================
	UPROPERTY()
	TObjectPtr<AActor> MainPhotoActor;
	
	UFUNCTION()
	void ClearMainPhotoActor(){MainPhotoActor = nullptr;}
	
	UFUNCTION()
		float GetPicturableScore();

	// ===============================
	// Zoom In & Out
	// ===============================
public:
	float TargetFOV = 90.f;
	float CurrentFOV = 90.f;
	float MinFOV = 30.f;
	float MaxFOV = 100.f;
	float ZoomStep = 10.f;
	float ZoomInterpSpeed = 10.f;

	
	
};
