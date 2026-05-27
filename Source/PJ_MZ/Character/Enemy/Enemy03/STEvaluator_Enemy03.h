// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Components/StateTreeAIComponent.h"
#include "STEvaluator_Enemy03.generated.h"

USTRUCT()
struct FSTEvaluator_Enemy03InstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Output)
	bool bPlayerDetected = false;

	UPROPERTY(EditAnywhere, Category=Output)
	bool bPlayerLooking = false;

	UPROPERTY(EditAnywhere, Category=Output)
	TObjectPtr<AActor> TargetActor = nullptr;
};

USTRUCT(DisplayName = "Enemy03 Evaluator")
struct  PJ_MZ_API FSTEvaluator_Enemy03 : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTEvaluator_Enemy03InstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	// 플레이어가 적을 바라보는 판정 각도
	UPROPERTY(EditAnywhere, Category = "AI")
	float LookAtAngleThreshold = 30.f;

	// 추적 포기 거리
	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseTargetDistance = 3000.f;
	
	TStateTreeExternalDataHandle<UStateTreeAIComponent> STComponentHandle;

private:
	bool IsPlayerLookingAtEnemy(AAIController& Controller) const;

	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
	TStateTreeExternalDataHandle<APawn> PawnHandle;};
