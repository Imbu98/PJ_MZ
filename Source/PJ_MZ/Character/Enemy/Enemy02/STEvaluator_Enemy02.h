#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "StateTreeExecutionTypes.h"
#include "STEvaluator_Enemy02.generated.h"

USTRUCT()
struct FSTEvaluator_Enemy02InstanceData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY()
	TObjectPtr<APawn> Pawn = nullptr;

	UPROPERTY(EditAnywhere, Category = Output)
	FVector SoundLocation = FVector::ZeroVector;
};


USTRUCT(meta = (DisplayName = "Enemy02 Evaluator"))
struct PJ_MZ_API FSTEvaluator_Enemy02 : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FSTEvaluator_Enemy02InstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}
	
	UPROPERTY(EditAnywhere, Category = "Config")
	float AttackRange = 200.f;

private:
	TStateTreeExternalDataHandle<AAIController> AIControllerHandle;
	TStateTreeExternalDataHandle<APawn> PawnHandle;
	TStateTreeExternalDataHandle<UStateTreeAIComponent> STComponentHandle;
};
