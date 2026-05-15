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
}

void AHT_Player::BeginPlay()
{
	Super::BeginPlay();

	// initialize sprint meter to max
	SprintMeter = SprintTime;

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHT_Player::SprintFixedTick, SprintFixedTickTime, true);
	
	// 정신력 초기화
	CurrentMentality = MaxMentality;
	
	// 플레이어컨트롤러 캐싱
	CachedHT_Pc = Cast<AHT_PlayerController>(Controller);
	
	// 불 끄기
	if (SpotLight)
	{
		SpotLight->SetVisibility(false);
	}
	
	//
	if (ObscuraCameraComp)
	{
		UMz_GameInstance* Mz_GI = Cast<UMz_GameInstance>(GetGameInstance());
		if (Mz_GI)
		{
			if (Mz_GI->bIsShotCountInitialized==false)
			{
				ObscuraCameraComp->InitShotCount();	
			}
			else
			{
				ObscuraCameraComp->CurrentCanShotCount= Mz_GI->CachedShotCount;
			}
			OnUpdateObscuraShotCount();
			
			Mz_GI->bIsShotCountInitialized=true;
		}

		
		
	
	}
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
	OnStaminaChangeDelegate.Broadcast(SprintMeter / SprintTime);

}

void AHT_Player::OnChangeMentality(float amount)
{
	CurrentMentality+=amount;
	
	OnMentalityChangeDelegate.Broadcast(CurrentMentality/MaxMentality);
}

void AHT_Player::OnInteractInput(const FInputActionValue& Value)
{
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
	if (!ObscuraCameraComp) return;
	if (ObscuraCameraComp->IsObscraCooltime)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("CooltimeBlocked"));
		return;
	}
	
	
		if (ObscuraCameraComp->GetObscuraMode()==EObscuraModeAction::CAMERAMODE)
		{
			ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::SHOTTING);
			if (SpotLight)
			{
				SpotLight->SetVisibility(true);
				
				if (CachedHT_Pc&&CachedHT_Pc->ObscuraUIWidget)
				{
					ObscuraCameraComp->IsObscraCooltime = true;	
				}
				
				
				// 화면 플래시 , todo: 찰칼소리
				AHT_PlayerController* pc = Cast<AHT_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
				if (pc)
				{
					pc->SetFadeOutWhiteUI();
				}
					
				FTimerHandle TimerHandle;
	
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.01f);
	
				GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this]()
				{
					SpotLight->SetVisibility(false);
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.f);
					ObscuraCameraComp->ApplyShutterDamage();
					ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::CAMERAMODE);
					OnUpdateObscuraShotCount();
					GetWorldTimerManager().ClearTimer(ObscuraTimer);
		
				},0.01f,false);
			}
		}
}

void AHT_Player::OnUpdateObscuraShotCount()
{
	if (ObscuraCameraComp)
	{
		int count = ObscuraCameraComp->CurrentCanShotCount;
		OnShotCountChangeDelegate.Broadcast(count);
	
		// GI에 저장
		UMz_GameInstance* Mz_GI = Cast<UMz_GameInstance>(GetGameInstance());
		if (Mz_GI)
		{
			Mz_GI->CachedShotCount = count; 
		}		
	}
}

void AHT_Player::OnEnterObscuraMode(const FInputActionValue& Value)
{
	// if (EquipObscuraMontage)
	// {
	// 	GetMesh()->GetAnimInstance()->Montage_Play(EquipObscuraMontage,1.0f);
	// }
	CreateObscuraWidget();
}

void AHT_Player::OnOutObscuraMode(const FInputActionValue& Value)
{
	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
 //    
	// if (AnimInstance && AnimInstance->Montage_IsPlaying(EquipObscuraMontage))
	// {
	// 	// 장착 몽타주 재생 중이었다면 중단
	// 	AnimInstance->Montage_Stop(0.2f, EquipObscuraMontage); // 0.2f = 블렌드아웃 시간
 //        
	// 	if (UnEquipObscuraMontage)
	// 	{
	// 		AnimInstance->Montage_Play(UnEquipObscuraMontage,-1.0f);
	// 	}
	// }
	if (ObscuraCameraComp)
	{
		ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::IDLE);
		GetWorldTimerManager().ClearTimer(ObscuraTimer);
		if (SpotLight->IsVisible())
		{
			SpotLight->SetVisibility(false);
		}
	}

	
	RemoveObscuraWidget();

}

void AHT_Player::CreateObscuraWidget()
{
	if (CachedHT_Pc)
	{
		CachedHT_Pc->CreateObscuraWidget();
		if (ObscuraCameraComp)
		{
			ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::CAMERAMODE);
			OnShotCountChangeDelegate.Broadcast(ObscuraCameraComp->CurrentCanShotCount);
		}
	}
}

void AHT_Player::RemoveObscuraWidget()
{
	if (CachedHT_Pc)
	{
		CachedHT_Pc->RemoveObscuraWidget();
		if (ObscuraCameraComp)
		{
			ObscuraCameraComp->SetObscuraMode(EObscuraModeAction::IDLE);
		}
	}
}
