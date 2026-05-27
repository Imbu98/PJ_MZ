#include "HT_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "HT_Player.h"
#include "HT_PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/ObscuraCameraComponent.h"
#include "UI/ControlFadeUI.h"
#include "UI/ObscuraUI.h"
#include "UI/PlayerStateUI.h"
#include "UI/ResultUI.h"

AHT_PlayerController::AHT_PlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AHT_PlayerController::StaticClass();
}

void AHT_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHT_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
		if (PlayerStateUIFactory)
    	{
    		PlayerStateUIWidget = CreateWidget<UPlayerStateUI>(this,PlayerStateUIFactory);
    		if (PlayerStateUIWidget)
    		{
    			PlayerStateUIWidget->AddToViewport();
    			auto* playerState =(GetPlayerState<AHT_PlayerState>());
    			if (playerState)
    			{
    				PlayerStateUIWidget->SetupCharacter(playerState);
    			}
    			
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

void AHT_PlayerController::CreateObscuraWidget()
{
	SetFadeOutBlackUI();
    
	if (PlayerStateUIWidget)
	{
		PlayerStateUIWidget->SetVisibility(ESlateVisibility::Hidden);
	}
    
	if (!ObscuraUIWidget)
	{
		if (ObscuraUIFactory)
		{
			ObscuraUIWidget = CreateWidget<UObscuraUI>(this, ObscuraUIFactory);
			if (ObscuraUIWidget)
			{
				ObscuraUIWidget->CameraObscuraComp = GetPawn()->FindComponentByClass<UObscuraCameraComponent>();
				ObscuraUIWidget->AddToViewport();
			}
		}
	}
    
	if (ObscuraUIWidget)
	{
		ObscuraUIWidget->SetVisibility(ESlateVisibility::Visible);
		ObscuraUIWidget->bDetectionActive = true;
	}
}

void AHT_PlayerController::RemoveObscuraWidget()
{
	SetFadeOutBlackUI();
		
	if (PlayerStateUIWidget)
	{
		PlayerStateUIWidget->SetVisibility(ESlateVisibility::Visible);	
	}
	
	if (ObscuraUIWidget&& ObscuraUIWidget->IsInViewport())
	{
		ObscuraUIWidget->SetVisibility(ESlateVisibility::Collapsed);
		ObscuraUIWidget->bDetectionActive = false;
	}
}

void AHT_PlayerController::SetFadeOutBlackUI()
{
	if (!ControlFadeUIFactory) return;

	if (ControlFadeUIWidget && ControlFadeUIWidget->IsInViewport())
	{
		// 이미 있으면 페이드아웃만 (제거는 애니메이션 완료 후)
		ControlFadeUIWidget->FadeOutBlack();
	}
	else
	{
		// 새로 생성
		UControlFadeUI* NewWidget = CreateWidget<UControlFadeUI>(this, ControlFadeUIFactory);

		if (NewWidget)
		{
			ControlFadeUIWidget = NewWidget;
			ControlFadeUIWidget->AddToViewport();
			ControlFadeUIWidget->FadeOutBlack();
		}
	}
}

void AHT_PlayerController::SetFadeOutWhiteUI()
{
	if (!ControlFadeUIFactory) return;

	if (ControlFadeUIWidget && ControlFadeUIWidget->IsInViewport())
	{
		// 이미 있으면 페이드아웃만 (제거는 애니메이션 완료 후)
		ControlFadeUIWidget->FadeOutWhite();
	}
	else
	{
		// 새로 생성
		UControlFadeUI* NewWidget = CreateWidget<UControlFadeUI>(this, ControlFadeUIFactory);

		if (NewWidget)
		{
			ControlFadeUIWidget = NewWidget;
			ControlFadeUIWidget->AddToViewport();
			ControlFadeUIWidget->FadeOutWhite();
		}
	}
}

void AHT_PlayerController::SetResultUI(const float totalScore)
{
	AHT_PlayerState* Ps=  GetPlayerState<AHT_PlayerState>();
	if (Ps==nullptr) return;

	if (PhotoResultUIFactory)
	{
		PhotoResultUIWidget = CreateWidget<UResultUI>(this, PhotoResultUIFactory);
		if (PhotoResultUIWidget)
		{
			PhotoResultUIWidget->AddToViewport();
			
				for (int32 i=0;i<Ps->MaxCanShotCount;i++)
				{
					FOwningPictureData PictureData = Ps->GetOwningPictureData(i);
					if (PictureData.PhotoImage!=nullptr)
					{
						PhotoResultUIWidget->SetPhotoImage(i,PictureData,Ps->GetFormattedTime());	
					}
				}
			// 최종 점수 표시
			PhotoResultUIWidget->SetTotalScoreText(totalScore);
		}
	}
}
