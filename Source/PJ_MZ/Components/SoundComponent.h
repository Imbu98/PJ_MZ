#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PJ_MZ_API USoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoundComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<AActor> owner;
	
	// ===============================
	// 카메라 셔터음
	// ===============================
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USoundBase> CameraShutterSound;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USoundBase> HorrorBGM;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> horrorBgmAudioComp;
	
	UPROPERTY()
	TObjectPtr<class UAudioComponent> BreathBgmAudioComp;
	
public:
	UFUNCTION()
	void PlayCameraShutterSound();
	
	UFUNCTION()
	void ControlHorrorBGM(bool soundOn);
	
	UFUNCTION()
	void ControlBreath(bool soundOn,USoundBase* sound);
};
