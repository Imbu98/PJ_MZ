#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObscuraUI.generated.h"

UCLASS()
class PJ_MZ_API UObscuraUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	

	virtual void NativeConstruct() override;
	
};
