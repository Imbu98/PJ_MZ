#pragma once
#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AIController.h"
#include "STTask_Investigate.generated.h"

USTRUCT()
struct FSTTask_Investigate_Enemy02InstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FVector SoundLocation = FVector::ZeroVector;

	float ElapsedTime = 0.f;
};

USTRUCT(meta = (DisplayName = "Enemy02 Investigate"))
struct FSTTask_Investigate : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTTask_Investigate_Enemy02InstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual EStateTreeRunStatus EnterState(
		FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(
		FStateTreeExecutionContext& Context,
		const float DeltaTime) const override;

	UPROPERTY(EditAnywhere, Category = "Config")
	float InvestigateTime = 20.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float MoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float WanderRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float AcceptanceRadius = 50.f;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};