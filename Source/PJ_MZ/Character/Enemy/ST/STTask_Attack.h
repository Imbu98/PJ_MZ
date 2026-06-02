#pragma once
#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AIController.h"
#include "STTask_Attack.generated.h"

USTRUCT()
struct FSTTask_AttackInstanceData
{
	GENERATED_BODY()
};

USTRUCT(meta = (DisplayName = "Enemy Attack"))
struct FSTTask_Attack : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTTask_AttackInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual bool Link(FStateTreeLinker& Linker) override;

	virtual EStateTreeRunStatus EnterState(
		FStateTreeExecutionContext& Context,
		const FStateTreeTransitionResult& Transition) const override;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};