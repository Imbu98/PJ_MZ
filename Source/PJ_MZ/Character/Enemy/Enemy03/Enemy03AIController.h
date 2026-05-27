
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Enemy02/Enemy02AIController.h"
#include "Enemy03AIController.generated.h"

UCLASS()
class PJ_MZ_API AEnemy03AIController : public AEnemy02AIController
{
	GENERATED_BODY()

public:
	AEnemy03AIController();
	
	bool bPlayerDetected = false;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	// 플레이어가 적을 바라보고 있는지 판정
	bool IsPlayerLookingAtMe() const;

	// 플레이어가 적을 바라보는 판정 각도
	UPROPERTY(EditAnywhere, Category = "AI")
	float LookAtAngleThreshold = 30.f;

	// 추적 포기 거리
	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseTargetDistance = 3000.f;
};
