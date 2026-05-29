#pragma once

#include "CoreMinimal.h"
#include "MZ_Datas.h"
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
	
	virtual void CopyProperties(class APlayerState* PlayerState) override;

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
	FOwningPictureData GetOwningPictureData(int32 index);

	// HT_PlayerState.h
	UPROPERTY(BlueprintReadWrite, Category = "Photo")
	TArray<FOwningPictureData> OwningPictureArray; // 기존 UTextureRenderTarget2D* 에서 변경!
	

	// ===============================
	// 아이디 및 닉네임 
	// ===============================
	UPROPERTY()
	FString MZ_PlayerID;

	UPROPERTY()
	FString MZ_PlayerName;
	
	
	// ===============================
	// 플레이어 플레이타임
	// ===============================
	UPROPERTY()
	float ElapsedSeconds = 0.f;

	float GetElapsedSeconds() const { return ElapsedSeconds; }
	
	FString GetFormattedTime() const;
	
	
	// ===============================
	// 사진 쿨타임
	// ===============================
	bool IsObscraCooltime = false;
	float currentObscuraCooltime = 0.f;
	float MaxObscuraCooltime = 1.f;

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
	FCameraFOVChangeDelegate OnCameraFOVChangeDelegate;

	
	
	
};