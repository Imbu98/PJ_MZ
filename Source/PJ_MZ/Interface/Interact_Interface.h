#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteract_Interface : public UInterface
{
	GENERATED_BODY()
};

class PJ_MZ_API IInteract_Interface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void Interacted();

public:
};
