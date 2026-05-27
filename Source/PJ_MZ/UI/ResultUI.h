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
	TObjectPtr<class UTextBlock> ScoreText1;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> PhotoImage2;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> ScoreText2;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> PhotoImage3;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> ScoreText3;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> Text_PlayTimeResult;
	
public:
	UFUNCTION()
	void SetPhotoImage(int32 index, UTexture2D* renderTarget,float scoreVal,const FString Formattedstring);
};
