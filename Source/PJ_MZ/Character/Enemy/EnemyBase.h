
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
	
	// UPROPERTY()
	// bool bResetDetection = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPicturableComponent> PicturableComp;
	
	UPROPERTY(EditAnywhere, Category="EnemySound")
	TObjectPtr<USoundAttenuation> SoundAttenuation;

	UPROPERTY(EditAnywhere, Category="EnemySound")
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditAnywhere, Category="EnemySound")
	TObjectPtr<USoundBase> StunSound;

	UPROPERTY(EditAnywhere, Category="EnemySound")
	TArray<TObjectPtr<USoundBase>> AmbientSounds;

	
	UPROPERTY(EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> StunMontage;
	
	
	UFUNCTION(BlueprintCallable)
	void StartAttack();
	UFUNCTION(BlueprintCallable)
	void StopAttack();

	bool IsAttacking() const
	{
		return bAttacking;
	}
	UFUNCTION(BlueprintCallable)
	void AttackHit();

	// UFUNCTION(BlueprintCallable)
	// void OnAttackSuccess();

	virtual void BeginPlay() override;
	
	void HaltMovement();
	
	FTimerHandle AmbientTimerHandle;

	void StartAmbientSounds();
	void StopAmbientSounds();
	void PlayRandomAmbientSound();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float RespawnDelay = 5.f;
	
	bool bAttacking = false;

	UFUNCTION()
	void OnAttackMontageEnded(
		UAnimMontage* Montage,
		bool bInterrupted);

private:
	void DistoryAndRequestRespawn();
};
