
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "Enemy02Character.generated.h"

UCLASS()
class PJ_MZ_API AEnemy02Character : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy02Character();
	virtual void Attack() override;

};
