#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultUI.generated.h"

UCLASS()
class PJ_MZ_API UResultUI : public UUserWidget
{
	GENERATED_BODY()
	
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
	
public:
	UFUNCTION()
	void SetPhotoImage(int32 index, const FOwningPictureData& pictureData,const FString& timeString,bool IsDuplicate);
	void SetTotalScoreText(float totalScore);
};
