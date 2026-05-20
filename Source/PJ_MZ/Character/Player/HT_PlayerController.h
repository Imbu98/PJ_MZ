#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HT_PlayerController.generated.h"

UCLASS()
class PJ_MZ_API AHT_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AHT_PlayerController();
	
public:
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<class UInputMappingContext*> DefaultMappingContexts;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UPlayerStateUI> PlayerStateUIFactory;
	
	UPROPERTY()
	TObjectPtr<class UPlayerStateUI> PlayerStateUIWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UObscuraUI> ObscuraUIFactory;
	
	UPROPERTY()
	TObjectPtr<class UObscuraUI> ObscuraUIWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UControlFadeUI> ControlFadeUIFactory;
	
	UPROPERTY()
	TObjectPtr<class UControlFadeUI> ControlFadeUIWidget;
	
	UFUNCTION()
	void CreateObscuraWidget();
	
	UFUNCTION()
	void RemoveObscuraWidget();
	
	UFUNCTION()
	void SetFadeOutBlackUI();
	
	UFUNCTION()
	void SetFadeOutWhiteUI();
	

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
};
