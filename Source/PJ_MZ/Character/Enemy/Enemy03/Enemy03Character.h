
#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "Enemy03Character.generated.h"

UCLASS()
class PJ_MZ_API AEnemy03Character : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy03Character();
	
	virtual void BeginPlay() override;

	// virtual void Attack() override;
	
	UPROPERTY(BlueprintReadOnly, Category="Animation")
	bool bIsAlerted = false;

	void SetAlerted(bool bInAlerted) { bIsAlerted = bInAlerted; }
	
	UPROPERTY(VisibleAnywhere, Category="EnemySound")
	TObjectPtr<UAudioComponent> ChaseAudioComp;

	UPROPERTY(EditAnywhere, Category="EnemySound")
	TObjectPtr<USoundBase> ChaseSound;
};
