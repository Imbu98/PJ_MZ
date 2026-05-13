#include "HT_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "HTCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UI/ControlFadeUI.h"
#include "UI/ObscuraUI.h"
#include "UI/PlayerStateUI.h"

AHT_PlayerController::AHT_PlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AHT_PlayerController::StaticClass();
}

void AHT_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerStateUIFactory)
	{
		PlayerStateUIWidget = Cast<UPlayerStateUI>(CreateWidget(this,PlayerStateUIFactory));
		if (PlayerStateUIWidget)
		{
			PlayerStateUIWidget->AddToViewport();
			auto* player = Cast<AHTCharacter>(this->GetPawn());
			if (player)
			{
				PlayerStateUIWidget->SetupCharacter(player);	
			}
			
		}
	}
	
}

void AHT_PlayerController::CreateObscuraWidget()
{
	SetFadeOutUI();
	if (PlayerStateUIWidget)
	{
		PlayerStateUIWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (!ObscuraUIWidget)
	{
		if (ObscuraUIFactory)
		{
			ObscuraUIWidget = Cast<UObscuraUI>(CreateWidget(this,ObscuraUIFactory));
			if (ObscuraUIWidget)
			{

				ObscuraUIWidget->AddToViewport();
			}
		}	
	}
	else
	{
		if (!ObscuraUIWidget->IsInViewport())
		{
			ObscuraUIWidget->AddToViewport();
		}
	}
	
	
}

void AHT_PlayerController::RemoveObscuraWidget()
{
	SetFadeOutUI();
	
	if (PlayerStateUIWidget)
	{
		PlayerStateUIWidget->SetVisibility(ESlateVisibility::Visible);	
	}
	
	if (ObscuraUIWidget->IsInViewport())
	{
		ObscuraUIWidget->RemoveFromParent();
	}
}

void AHT_PlayerController::SetFadeOutUI()
{
	if (!ControlFadeUIFactory) return;

	if (ControlFadeUIWidget && ControlFadeUIWidget->IsInViewport())
	{
		// 이미 있으면 페이드아웃만 (제거는 애니메이션 완료 후)
		ControlFadeUIWidget->FadeOut();
	}
	else
	{
		// 새로 생성
		UControlFadeUI* NewWidget = Cast<UControlFadeUI>(
			CreateWidget(this, ControlFadeUIFactory)
		);

		if (NewWidget)
		{
			ControlFadeUIWidget = NewWidget;
			ControlFadeUIWidget->AddToViewport();
			ControlFadeUIWidget->FadeOut();
		}
	}
}



void AHT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
	
}