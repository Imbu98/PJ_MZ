#pragma once
#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AIController.h"
#include "STTask_MoveToSound.generated.h"

USTRUCT()
struct FSTTask_MoveToSound_InstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FVector SoundLocation = FVector::ZeroVector;
};

USTRUCT(meta = (DisplayName = "Enemy02 MoveToSound"))
struct FSTTask_MoveToSound : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTTask_MoveToSound_InstanceData;

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
	float MoveSpeed = 700.f;

	UPROPERTY(EditAnywhere, Category = "Config")
	float AcceptanceRadius = 50.f;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};
