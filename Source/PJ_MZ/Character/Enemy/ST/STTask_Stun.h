#pragma once
#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AIController.h"
#include "STTask_Stun.generated.h"

USTRUCT()
struct FSTTask_StunInstanceData
{
	GENERATED_BODY()

	float StunTimer = 0.f;
};

USTRUCT(meta = (DisplayName = "Enemy Stun"))
struct FSTTask_Stun : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTTask_StunInstanceData;

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

	virtual void ExitState(
		FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) const override;

	UPROPERTY(EditAnywhere, Category = "Config")
	float StunDuration = 2.f;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};