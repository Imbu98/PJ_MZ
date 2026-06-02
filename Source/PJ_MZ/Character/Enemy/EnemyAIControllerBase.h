
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyAIControllerBase.generated.h"

UCLASS()
class PJ_MZ_API AEnemyAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIControllerBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float HearingRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float HearingMaxAge = 5.f;

public:
	// 사진 찍힐 때 불릴 함수
	void PictureTaken();
};
