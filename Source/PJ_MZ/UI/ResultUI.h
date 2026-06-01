#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultUI.generated.h"

UCLASS()
class PJ_MZ_API UResultUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UImage> PhotoImage1;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_Score1;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_ObjectName1;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> PhotoImage2;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_Score2;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_ObjectName2;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> PhotoImage3;
			
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_Score3;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_ObjectName3;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_PlayTimeResult;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_ScoreResult;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UButton> Button_ShowLeaderboard;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UOverlay> Overlay_Result1;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UOverlay> Overlay_Result2;
	
	

	
	
public:
	UFUNCTION()
	void SetPhotoImage(int32 index, const FOwningPictureData& pictureData,const FString& timeString,bool IsDuplicate);
	void SetTotalScoreText(float totalScore);
	
	UFUNCTION()
	void OnClickedButton_ShowLeaderboard();
};
