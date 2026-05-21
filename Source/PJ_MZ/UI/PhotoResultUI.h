#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PhotoResultUI.generated.h"

UCLASS()
class PJ_MZ_API UPhotoResultUI : public UUserWidget
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
	
public:
	UFUNCTION()
	void SetPhotoImage(int32 index, UTextureRenderTarget2D* renderTarget,float scoreVal);
};
