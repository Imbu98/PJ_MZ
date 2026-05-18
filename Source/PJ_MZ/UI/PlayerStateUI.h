// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateUI.generated.h"

class AHT_Player;

/**
 *  Simple UI for a first person horror game
 *  Manages character sprint meter display
 */
UCLASS(abstract)
class PJ_MZ_API UPlayerStateUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** Sets up delegate listeners for the passed character */
	void SetupCharacter(class AHT_PlayerState* playerState);
	
	UFUNCTION()
	void OnPlayerMentalityBarUpdated(float percent);
	
	// 스프린트 게이지
	UFUNCTION()
	void OnStaminaBarUpdated(float percent);
	
	UFUNCTION()
	void OnShotCountUpdated(int shotCount);
	
private:
	// player 정신력수치 프로그레스 바
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> PB_MentalityBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> PB_StaminaBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UHorizontalBox> ObscuraCountHorizontalBox;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget> ObscuraCountFactory;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> ObscuraCountWidget;
	
	
};
