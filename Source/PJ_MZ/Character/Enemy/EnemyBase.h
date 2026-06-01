
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class PJ_MZ_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPicturableComponent> PicturableComp;
	
	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	void OnAttackSuccess();

	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float RespawnDelay = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	UFUNCTION()
	virtual void OnSeePawn(APawn* SensedPawn);

private:
	void DistoryAndRequestRespawn();
};
