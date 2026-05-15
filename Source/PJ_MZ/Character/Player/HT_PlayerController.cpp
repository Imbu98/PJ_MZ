#include "HT_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "HT_Player.h"
#include "Blueprint/UserWidget.h"
#include "Components/ObscuraCameraComponent.h"
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
		PlayerStateUIWidget = CreateWidget<UPlayerStateUI>(this,PlayerStateUIFactory);
		if (PlayerStateUIWidget)
		{
			PlayerStateUIWidget->AddToViewport();
			auto* player = Cast<AHT_Player>(this->GetPawn());
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
		PlayerStateUIWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (!ObscuraUIWidget)
	{
		if (ObscuraUIFactory)
		{
			ObscuraUIWidget = CreateWidget<UObscuraUI>(this,ObscuraUIFactory);
			if (ObscuraUIWidget)
			{
				ObscuraUIWidget->CameraObscuraComp = GetPawn()->FindComponentByClass<UObscuraCameraComponent>();

				ObscuraUIWidget->AddToViewport();
				ObscuraUIWidget->bDetectionActive = true; // 감지 시작
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
	
	if (ObscuraUIWidget&&ObscuraUIWidget->IsInViewport())
	{
		ObscuraUIWidget->RemoveFromParent();
		ObscuraUIWidget->bDetectionActive = false; // 감지 중단
		ObscuraUIWidget = nullptr;
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
		UControlFadeUI* NewWidget = CreateWidget<UControlFadeUI>(this, ControlFadeUIFactory);

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