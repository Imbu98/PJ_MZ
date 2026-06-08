
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
	
	UPROPERTY(VisibleAnywhere, Category="Sound")
	TObjectPtr<UAudioComponent> BreathAudioComp;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<USoundBase> BreathSound;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	TObjectPtr<USoundBase> DetectionSound;
	
	virtual void BeginPlay() override;
};
