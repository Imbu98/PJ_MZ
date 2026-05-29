#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoginPlayerController.generated.h"

UCLASS()
class PJ_MZ_API ALoginPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	ALoginPlayerController();
	
	virtual void BeginPlay() override;
	
public:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULoginUI> LoginUIFactory;
	
	UPROPERTY()
	TObjectPtr<ULoginUI> LoginUIWidget;
	
};
