
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTask_FreezePlayer.generated.h"


USTRUCT()
struct FFreezePlayerInstanceData
{
	GENERATED_BODY()
};

USTRUCT(DisplayName = "Freeze Player")
struct PJ_MZ_API FSTTask_FreezePlayer : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FFreezePlayerInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	UPROPERTY(EditAnywhere, Category = "Freeze")
	float FrozenSpeed = 0.f;

private:
	TStateTreeExternalDataHandle<AAIController> ControllerHandle;
};
