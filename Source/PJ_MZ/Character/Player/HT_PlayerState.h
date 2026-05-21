#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PJ_MZ_Delegates.h"
#include "HT_PlayerState.generated.h"

UCLASS()
class PJ_MZ_API AHT_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	virtual  void Tick(float DeltaTime) override;
	
	AHT_PlayerState();

public:

	// ===============================
	// 촬영 횟수
	// ===============================
	UPROPERTY()
	int32 MaxCanShotCount = 3;

	UPROPERTY()
	int32 CurrentCanShotCount;
	
	void InitShotCount() { CurrentCanShotCount = MaxCanShotCount; }
	void SetCurrentCanShotCount(int32 newCount) { CurrentCanShotCount = newCount; }
	int32 GetCurrentCanShotCount() { return CurrentCanShotCount; }
	
	// ===============================
	// 점수 및 사진 저장
	// ===============================
	UPROPERTY()
	TArray<float> CachedScoreArray;
	
	float GetCachedScore(int32 index);

	UPROPERTY()
	TArray<UTextureRenderTarget2D*> PhotoList;	// 찍은 사진들 저장
	
	UTextureRenderTarget2D* GetPhotoTexture(int32 index);
	
	// ===============================
	// 플레이어 플레이타임
	// ===============================
	UPROPERTY()
	float ElapsedSeconds = 0.f;
	
	FString GetFormattedTime() const;
	
	
	// ===============================
	// 사진 쿨타임
	// ===============================
	bool IsObscraCooltime = false;
	float currentObscuraCooltime = 0.f;
	float MaxObscuraCooltime = 3.f;

	// ===============================
	// 플레이어 정신력
	// ===============================
	UPROPERTY(EditAnywhere, Category="Mentality")
	float MaxMentality = 100.f;
	UPROPERTY()
	float CurrentMentality;

	// ===============================
	// 델리게이트
	// ===============================
	FStaminaChangeDelegate OnStaminaBarUpdated;
	FMentalityChangedDelegate OnMentalityChangeDelegate;
	FShotCountChangeDelegate OnShotCountChangeDelegate;
	FObscuraCooltimeFinished OnObscuraCooltimeFinished;

	
	
	
};