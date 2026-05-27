
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
	
	// 공격 - 자식 클래스에서 오버라이드 가능
	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	// 공격 성공 후 사라지기
	UFUNCTION(BlueprintCallable)
	void OnAttackSuccess();

protected:
	virtual void BeginPlay() override;
	
	// 재스폰까지 대기 시간 - 에디터에서 조정 가능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float RespawnDelay = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	UFUNCTION()
	void OnSeePawn(APawn* SensedPawn);


private:
	void DistoryAndRequestRespawn();
};
