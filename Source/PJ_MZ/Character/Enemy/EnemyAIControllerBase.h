// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Hearing.h"
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

	// 자식 클래스에서 BT/ST 에셋 경로 지정
	virtual FString GetBehaviorTreePath() const { return TEXT(""); }
	virtual FString GetBlackboardPath() const { return TEXT(""); }

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAISenseConfig_Hearing* HearingConfig;

	// 에디터에서 조정 가능한 청각 설정
	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float HearingRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "AI|Hearing")
	float HearingMaxAge = 5.f;

	// Blackboard 키 이름 - 공통
	const FName BB_SoundLocation = TEXT("SoundLocation");
	const FName BB_TargetActor   = TEXT("TargetActor");
};
