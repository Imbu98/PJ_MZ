// Copyright Epic Games, Inc. All Rights Reserved.


#include "HTCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "HT_PlayerController.h"
#include "InputAction.h"
#include "Interface/Interact_Interface.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

AHTCharacter::AHTCharacter()
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
}

void AHTCharacter::BeginPlay()
{
	Super::BeginPlay();

	// initialize sprint meter to max
	SprintMeter = SprintTime;

	// Initialize the walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// start the sprint tick timer
	GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &AHTCharacter::SprintFixedTick, SprintFixedTickTime, true);
	
	// 정신력 초기화
	CurrentMentality = MaxMentality;
}

void AHTCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the sprint timer
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
}

void AHTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// Sprinting
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHTCharacter::DoStartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHTCharacter::DoEndSprint);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHTCharacter::OnInteractInput);
			
			EnhancedInputComponent->BindAction(EnterCameraModeAction, ETriggerEvent::Started, this, &AHTCharacter::OnEnterCameraMode);
			EnhancedInputComponent->BindAction(EnterCameraModeAction, ETriggerEvent::Completed, this, &AHTCharacter::OnOutCameraMode);
			
			

		}
	}
}

void AHTCharacter::DoStartSprint()
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

void AHTCharacter::DoEndSprint()
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

void AHTCharacter::SprintFixedTick()
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

void AHTCharacter::OnChangeMentality(float amount)
{
	CurrentMentality+=amount;
	
	OnMentalityChangeDelegate.Broadcast(CurrentMentality/MaxMentality);
}

void AHTCharacter::OnInteractInput(const FInputActionValue& Value)
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

void AHTCharacter::OnEnterCameraMode(const FInputActionValue& Value)
{
	if (AHT_PlayerController* ht_pc = Cast<AHT_PlayerController>(Controller))
	{
		ht_pc->CreateObscuraWidget();
	}
}

void AHTCharacter::OnOutCameraMode(const FInputActionValue& Value)
{
	if (AHT_PlayerController* ht_pc = Cast<AHT_PlayerController>(Controller))
	{
		ht_pc->RemoveObscuraWidget();
	}
	
}
