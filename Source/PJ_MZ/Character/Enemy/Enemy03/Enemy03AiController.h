
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy02/Enemy02AIController.h"
#include "Enemy03AiController.generated.h"

UCLASS()
class PJ_MZ_API AEnemy03AiController : public AEnemy02AIController
{
	GENERATED_BODY()

public:
	AEnemy03AiController();

protected:
	virtual void BeginPlay() override;
	
};
