
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "Components/StateTreeComponent.h"
#include "Enemy03Character.generated.h"

UCLASS()
class PJ_MZ_API AEnemy03Character : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy03Character();

	virtual void Attack() override;

protected:
	virtual void OnSeePawn(APawn* SensedPawn) override;
};
