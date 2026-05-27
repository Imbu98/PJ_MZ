
#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "STTask_RandomPatrol.generated.h"

class AAIController;

USTRUCT()
struct FRandomPatrolInstanceData
{
	GENERATED_BODY()

	UPROPERTY()
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY()
	float WaitTimer = 0.f;
};

USTRUCT(DisplayName = "Random Patrol")
struct PJ_MZ_API FSTTask_RandomPatrol : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FRandomPatrolInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float WaitTime = 2.f;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};
