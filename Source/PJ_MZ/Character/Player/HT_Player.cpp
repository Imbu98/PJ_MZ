// Copyright Epic Games, Inc. All Rights Reserved.


#include "HT_Player.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "HT_PlayerController.h"
#include "InputAction.h"
#include "Mz_GameInstance.h"
#include "Interface/Interact_Interface.h"
#include "Blueprint/UserWidget.h"
#include "Components/ObscuraCameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ObscuraUI.h"
#include "HT_PlayerState.h"
#include "ImageUtils.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SoundComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "UI/DialogueUI.h"

AHT_Player::AHT_Player()
{
	// create the spotlight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(GetFirstPersonCameraComponent());

	SpotLight->SetRelativeLocationAndRotation(FVector(30.0f, 17.5f, -5.0f), FRotator(-18.6f, -1.3f, 5.26f));
	SpotLight->Intensity = 0.5;
	SpotLight->SetIntensityUnits(ELightUnits::Lumens);
	SpotLight->AttenuationRadius = 1050.0f;
	SpotLight->InnerConeAngle = 18.7f;
	SpotLight->OuterConeAngle = 45.24f;
	
	ObscuraCameraComp = CreateDefaultSubobject<UObscuraCameraComponent>(TEXT("ObscuraCameraComp"));
	
	SoundComp = CreateDefaultSubobject<USoundComponent>(TEXT("SoundComp"));

	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitAutoFormat(1920, 1080);
}

void AHT_Player::BeginPlay()
{
	Super::BeginPlay();

	SprintMeter = SprintTime;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHT_Player::SprintFixedTick, SprintFixedTickTime, true);

	Cached_Pc = Cast<AHT_PlayerController>(Controller);
    
	// PlayerState 캐싱
	Cached_PS = GetPlayerState<AHT_PlayerState>();

	if (SpotLight)
	{
		SpotLight->SetVisibility(true);
	}

	//
	Cached_PS->CurrentMentality = Cached_PS->MaxMentality;
	Cached_PS->OnMentalityChangeDelegate.Broadcast(Cached_PS->CurrentMentality / Cached_PS->MaxMentality);
	
	
}

void AHT_Player::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the sprint timer
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}

void AHT_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Sprinting
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHT_Player::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHT_Player::DoEndSprint);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHT_Player::OnInteractInput);
			
			EnhancedInputComponent->BindAction(EnterCameraModeAction, ETriggerEvent::Started, this, &AHT_Player::OnEnterObscuraMode);
			EnhancedInputComponent->BindAction(EnterCameraModeAction, ETriggerEvent::Completed, this, &AHT_Player::OnOutObscuraMode);
			
			EnhancedInputComponent->BindAction(ShotObscuraAction, ETriggerEvent::Completed, this, &AHT_Player::OnShotObscura);

			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AHT_Player::OnZoomObscura);
			
		}
	}
}

void AHT_Player::DoStartSprint()
{
	// set the sprinting flag
	bSprinting = true;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the sprint walk speed
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	
	PlayerAbilityTags.AddTag(ShiftTag);

}

void AHT_Player::DoEndSprint()
{
	// set the sprinting flag
	bSprinting = false;

	// are we out of recovery mode?
	if (!bRecovering)
	{
		// set the default walk speed
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	
	PlayerAbilityTags.RemoveTag(ShiftTag);
}

void AHT_Player::SprintFixedTick()
{
	// are we out of recovery, still have stamina and are moving faster than our walk speed?
	if (bSprinting && !bRecovering && GetVelocity().Length() > WalkSpeed)
	{

		// do we still have meter to burn?
		if (SprintMeter > 0.0f)
		{
			// update the sprint meter
			SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);

			// have we run out of stamina?
			if (SprintMeter <= 0.0f)
			{
				// raise the recovering flag
				bRecovering = true;

				// set the recovering walk speed
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}
		
	} else {

		// recover stamina
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);

		if (SprintMeter >= SprintTime)
		{
			// lower the recovering flag
			bRecovering = false;

			// set the walk or sprint speed depending on whether the sprint button is down
			GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
		}

	}

	// broadcast the sprint meter updated delegate
	AHT_PlayerState* playerState = GetPlayerState<AHT_PlayerState>();
	if (playerState)
	{
		playerState->OnStaminaBarUpdated.Broadcast(SprintMeter / SprintTime);
	}
	

}

void AHT_Player::OnChangeMentality(float amount)
{
	if (!Cached_PS) return;
	Cached_PS->CurrentMentality += amount;
	
	Cached_PS->OnMentalityChangeDelegate.Broadcast(
		Cached_PS->CurrentMentality / Cached_PS->MaxMentality
	);
	
	// 정신력이 절반 이하로 떨어지면
	if (Cached_PS->CurrentMentality< Cached_PS->MaxMentality/2)
	{
		if (SoundComp)
		{
			SoundComp->PlayHorrorBGM();
		}
	}
}

void AHT_Player::OnInteractInput(const FInputActionValue& Value)
{
	if (IsPlayerStunned()) return;
	
	if (!Cached_Pc) return;
	
	// 기본상태가 아니면 상호작용 할 수 없게 변경
	if (ObscuraCameraComp->GetObscuraMode()!=EObscuraModeAction::IDLE) return;
	
	// 대화 중이면 다음 문장
	if (PlayerAbilityTags.HasTag(DialogTag))
	{  
		if (PlayerAbilityTags.HasTag(ShiftTag))
		{
			Cached_Pc->DialogueWidget->SkipDiaglogue();
		}
		else
		{
			Cached_Pc->DialogueWidget->OnNextInput();
		}
		return;
	}

	// 아니면 기존 상호작용
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;
	bool isHit = UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), GetActorLocation(), GetActorLocation(),200.f,
		TEXT("Interactable"),false,actorsToIgnore,EDrawDebugTrace::ForDuration,outHit,
		true,FLinearColor::Red);
	
	if (isHit)
	{
		AActor* hitActor = outHit.GetActor();
		if (hitActor&&hitActor->GetClass()->ImplementsInterface(UInteract_Interface::StaticClass()))
		{
			IInteract_Interface::Execute_Interacted(hitActor);
		}
	}
}

void AHT_Player::OnShotObscura(const FInputActionValue& Value)
{
	if (IsPlayerStunned()) return;
	if (!ObscuraCameraComp || !Cached_PS) return;
	if (Cached_PS->IsObscraCooltime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("CooltimeBlocked"));
		return;
	}

	if (ObscuraCameraComp->GetObscuraMode() == EObscuraModeAction::CAMERAMODE)
	{
		ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::SHOTTING);
		if (SpotLight)
		{
			SpotLight->SetVisibility(true);

			if (Cached_Pc && Cached_Pc->ObscuraUIWidget)
			{
				Cached_PS->IsObscraCooltime = true;
			}

			AHT_PlayerController* PC = Cast<AHT_PlayerController>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0)
			);
			if (PC)
			{
				PC->SetFadeOutWhiteUI();
				if (SoundComp)
				{
					SoundComp->PlayCameraShutterSound();	
				}
			}

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);

			GetWorld()->GetTimerManager().SetTimer(ObscuraTimer, [this]()
			{
				//SpotLight->SetVisibility(false);
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
				ObscuraCameraComp->ApplyShutterDamage();
				// 타이머 콜백 내부
				ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::CAMERAMODE);
				OnUpdateObscuraShotCount();

				if (bObscuraReleased) // 촬영 중에 우클릭을 뗐었다면
				{
					OnOutObscuraMode(FInputActionValue()); // 이번엔 SHOTTING 아니므로 정상 동작
				}
				
				GetWorldTimerManager().ClearTimer(ObscuraTimer);

			}, 0.01f, false);
		}
	}
}

void AHT_Player::OnUpdateObscuraShotCount()
{
	if (!Cached_PS) return;
	
	Cached_PS->OnShotCountChangeDelegate.Broadcast(Cached_PS->GetCurrentCanShotCount());

	// 게임인스턴스에 저장
	UMz_GameInstance* Mz_GI = Cast<UMz_GameInstance>(GetWorld()->GetGameInstance());
	if (Mz_GI)
	{
		Mz_GI->CachedShotCount = Cached_PS->GetCurrentCanShotCount();
	}
	
}

void AHT_Player::OnEnterObscuraMode(const FInputActionValue& Value)
{
	// if (EquipObscuraMontage)
	// {
	// 	GetMesh()->GetAnimInstance()->Montage_Play(EquipObscuraMontage,1.0f);
	// }

	// 카메라 모드 능력이 없으면 return
	if (IsPlayerStunned()) return;
	
	if (!PlayerAbilityTags.HasTag(ObscuraTag)) return;
		
	CreateObscuraWidget();	
}

void AHT_Player::OnOutObscuraMode(const FInputActionValue& Value)
{
	if (IsPlayerStunned()) return;
	// 카메라 모드 능력이 없으면 return
	if (!PlayerAbilityTags.HasTag(ObscuraTag)) return;

	if (ObscuraCameraComp==nullptr) return;


	if (ObscuraCameraComp->GetObscuraMode() == EObscuraModeAction::SHOTTING)
	{
		// 촬영 중에 우클릭을 뗐다면 플래그만 세워두고 대기
		bObscuraReleased = true;
		return;
	}

	ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::IDLE);

	// if (SpotLight->IsVisible())
	// 	SpotLight->SetVisibility(false);
	RemoveObscuraWidget();
    
	bObscuraReleased = false;

	// 카메라 줌 초기화
	GetFirstPersonCameraComponent()->SetFieldOfView(90.f);
}

void AHT_Player::OnZoomObscura(const FInputActionValue& Value)
{
	if (IsPlayerStunned()) return;
	
	if (!ObscuraCameraComp) return;
	if (ObscuraCameraComp->GetObscuraMode() != EObscuraModeAction::CAMERAMODE) return;

	float ScrollValue = Value.Get<float>();

	float targetFOV = FMath::Clamp(
		ObscuraCameraComp->TargetFOV - ScrollValue * ObscuraCameraComp->ZoomStep,
		ObscuraCameraComp->MinFOV,
		ObscuraCameraComp->MaxFOV
	);
	// TargetFOV만 변경
	ObscuraCameraComp->TargetFOV = targetFOV;
	
	// FOV변경 시 델리게이트 알림
	Cached_PS->OnCameraFOVChangeDelegate.Broadcast(targetFOV);
}

void AHT_Player::CreateObscuraWidget()
{
	

	if (Cached_Pc && Cached_PS)
	{
		Cached_Pc->CreateObscuraWidget();
		if (ObscuraCameraComp)
		{
			ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::CAMERAMODE);
			Cached_PS->OnShotCountChangeDelegate.Broadcast(Cached_PS->CurrentCanShotCount);
		}
	}
}

void AHT_Player::RemoveObscuraWidget()
{
	if (IsPlayerStunned()) return;
	
	if (Cached_Pc)
	{
		Cached_Pc->RemoveObscuraWidget();
		if (ObscuraCameraComp)
		{
			ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::IDLE);
			ObscuraCameraComp->ClearMainPhotoActor();
		}
	}
}

void AHT_Player::playerAttacked(float amount,float stunTime)
{
	PlayerAbilityTags.AddTag(StunTag);
	
	if (Cached_PS)
	{
		Cached_PS->ChangeMentality(amount);
	}
	
	if (AM_PlayerStun)
	{
		FirstPersonMesh->GetAnimInstance()->Montage_Play(AM_PlayerStun);
		
		// 시점 정면으로
		FRotator NewRot = Controller->GetControlRotation();
		NewRot.Pitch = 0.f;
		Controller->SetControlRotation(NewRot);
		
		GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
		
		// stunTime만큼 stun적용 후 태그 제거
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(
			this,
			&AHT_Player::EndState,
			StunTag);

		GetWorld()->GetTimerManager().SetTimer(
			StunTimerHandle,
			TimerDelegate,
			stunTime,
			false);
	}
}

void AHT_Player::EndState(FGameplayTag tag)
{
	PlayerAbilityTags.RemoveTag(tag);
}

void AHT_Player::PlayMontageOnCompleted(UAnimMontage* Montage, FOnMontageEnded MontageEndDelegate)
{
	if (Montage)	
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndDelegate,Montage);
	}
}


