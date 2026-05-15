// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../../Default/PJ_MZCharacter.h"
#include "PJ_MZ_Delegates.h"
#include "Interface/Interact_Interface.h"
#include "HT_Player.generated.h"

class AHT_PlayerController;
class USpotLightComponent;
class UInputAction;



/**
 *  Simple first person horror character
 *  Provides stamina-based sprinting
 */
UCLASS(abstract)
class PJ_MZ_API AHT_Player : public APJ_MZCharacter , public IInteract_Interface
{
	GENERATED_BODY()

	/** Player light source */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLight;
	
	UPROPERTY()
	TObjectPtr<class UObscuraCameraComponent> ObscuraCameraComp;
	
protected:

	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> SprintAction;
	
	// 카메라 촬영
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> ShotObscuraAction;
	
	// 상호작용 InputAction
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> InteractAction;
	
	// 카메라 촬영모드 진입
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> EnterCameraModeAction;

	/** If true, we're sprinting */
	bool bSprinting = false;

	/** If true, we're recovering stamina */
	bool bRecovering = false;

	/** Default walk speed when not sprinting or recovering */
	UPROPERTY(EditAnywhere, Category="Walk")
	float WalkSpeed = 150.0f;

	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float SprintFixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 3.0f;

	/** Walk speed while sprinting */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float SprintSpeed = 250.0f;

	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	/** Sprint tick timer */
	FTimerHandle SprintTimer;
	
	// Player 최대 정신력
	UPROPERTY(EditAnywhere,Category="Mentality")
	float MaxMentality = 100.f;
	
	// Player 현재 정신력
	UPROPERTY()
	float CurrentMentality;
	
	// PlayerController
	UPROPERTY()
	TObjectPtr<class AHT_PlayerController> CachedHT_Pc;
	
	// Player Obscura Mode Montage
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TObjectPtr<class UAnimMontage> EquipObscuraMontage;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TObjectPtr<UAnimMontage> UnEquipObscuraMontage;
	//
	

public:

	/** Delegate called when we start and stop sprinting */
	FStaminaChangeDelegate OnStaminaChangeDelegate;
	
	// 정신력 감소 델리게이트
	FMentalityChangedDelegate OnMentalityChangeDelegate;
	
	// 촬영 시 촬영 횟수 감소 UI 델리게이트
	FShotCountChangeDelegate OnShotCountChangeDelegate;

protected:

	/** Constructor */
	AHT_Player();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Gameplay cleanup */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

protected:

	/** Starts sprinting behavior */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartSprint();

	/** Stops sprinting behavior */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoEndSprint();

	/** Called while sprinting at a fixed time interval */
	void SprintFixedTick();
	
	// 플레이어 정신력 변화 함수
	UFUNCTION()
	void OnChangeMentality(float amount);
	
	UFUNCTION()
	void OnInteractInput(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnShotObscura(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnUpdateObscuraShotCount();
	
public:
	UFUNCTION()
	void OnEnterObscuraMode(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnOutObscuraMode(const FInputActionValue& Value);
	
	UFUNCTION()
	void CreateObscuraWidget();
	
	UFUNCTION()
	void RemoveObscuraWidget();
	
};
