
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy02/Enemy02AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "Enemy03AIController.generated.h"

UCLASS()
class PJ_MZ_API AEnemy03AIController : public AEnemy02AIController
{
	GENERATED_BODY()

public:
	AEnemy03AIController();
	
	UPROPERTY()
	bool bResetDetection = false;
	
	bool IsEnemyLookingAtPlayer() const;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Config")
	float EnemySightAngle = 60.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStateTreeAIComponent> StateTreeAIComponent;
};
