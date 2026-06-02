
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTask_ChasePlayer.generated.h"


USTRUCT()
struct FSTTask_ChasePlayerInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Input)
	float AttackRange = 200.f;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor = nullptr;
};

USTRUCT(DisplayName = "Chase Player")
struct PJ_MZ_API FSTTask_ChasePlayer : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTTask_ChasePlayerInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	UPROPERTY(EditAnywhere, Category = "Chase")
	float ChaseSpeed = 700.f;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};
