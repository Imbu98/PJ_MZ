 #include "HT_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "HT_Player.h"
#include "HT_PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/DynamoDBComponent.h"
#include "Components/ObscuraCameraComponent.h"
#include "../../Framework/PJ_MZGameMode.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Public/UI/TutorialUI.h"
#include "UI/ControlFadeUI.h"
#include "UI/ObscuraUI.h"
#include "UI/PlayerStateUI.h"
#include "UI/ResultUI.h"
#include "UI/ScoreLeaderboardUI.h"

AHT_PlayerController::AHT_PlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AHT_PlayerController::StaticClass();
}



void AHT_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
	
	APJ_MZGameMode* GameMode = Cast<APJ_MZGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// 델리게이트 바인딩
		GameMode->DynamoDBComp->OnLeaderboardFetched.AddUObject(this, &AHT_PlayerController::OnLeaderboardReceived);
	}

	// 게임 시작 시 팝업 미리 생성해두고 숨겨놓기
	if (PopupWidgetClass)
	{
		PopupWidget = CreateWidget<UPopupUI>(this, PopupWidgetClass);
		PopupWidget->AddToViewport();
		PopupWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 게임 시작 시 대화창 미리 생성해두고 숨겨놓기
	if (DialogueWidgetClass)
	{
		DialogueWidget = CreateWidget<UDialogueUI>(this, DialogueWidgetClass);
		DialogueWidget->AddToViewport();
		DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void AHT_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	CachedPlayer = Cast<AHT_Player>(InPawn);
	
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

void AHT_PlayerController::ShowPopup(FText Message, FText ConfirmText, FText CancelText,const FOnPopupAction& OnConfirm, const FOnPopupAction& OnCancel)
{
	if (!PopupWidget) return;
	
	if (CachedPlayer)
	{
		CachedPlayer->PlayerAbilityTags.AddTag(CachedPlayer->DialogTag);
	}
	PopupWidget->InitPopup(Message, ConfirmText, CancelText, OnConfirm, OnCancel);
	PopupWidget->SetVisibility(ESlateVisibility::Visible);

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
}

void AHT_PlayerController::HidePopup()
{
	if (!PopupWidget) return;
	
	if (CachedPlayer)
	{
		CachedPlayer->PlayerAbilityTags.RemoveTag(CachedPlayer->DialogTag);
	}

	PopupWidget->SetVisibility(ESlateVisibility::Collapsed);
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}

void AHT_PlayerController::ShowDialogue(const TArray<FDialogueLine>& Lines, const FOnDialogueFinished& OnFinished)
{
	if (!DialogueWidget) return;
	
	if (CachedPlayer)
	{
		CachedPlayer->PlayerAbilityTags.AddTag(CachedPlayer->DialogTag);
	}

	DialogueWidget->InitDialogue(Lines, OnFinished);
	DialogueWidget->SetVisibility(ESlateVisibility::Visible);
}

void AHT_PlayerController::HideDialogue()
{
	if (!DialogueWidget) return;
	
	if (CachedPlayer)
	{
		CachedPlayer->PlayerAbilityTags.RemoveTag(CachedPlayer->DialogTag);
	}
	
	DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AHT_PlayerController::SetResultUI(const float totalScore)
{
	AHT_PlayerState* Ps=  GetPlayerState<AHT_PlayerState>();
	if (Ps==nullptr) return;
	
	SetInputMode(FInputModeUIOnly());
	
	if (PhotoResultUIFactory)
	{
		PhotoResultUIWidget = CreateWidget<UResultUI>(this, PhotoResultUIFactory);
		if (PhotoResultUIWidget)
		{
			PhotoResultUIWidget->AddToViewport();
			
				for (int32 i=0;i<Ps->MaxCanShotCount;i++)
				{
					const FOwningPictureData& Data =Ps->GetOwningPictureData(i);
					bool bIsDuplicate = Data.IsDuplicate;
					PhotoResultUIWidget->SetPhotoImage(i, Data, Ps->GetFormattedTime(), bIsDuplicate);
				}
			// 최종 점수 표시
			PhotoResultUIWidget->SetTotalScoreText(totalScore);
		}
	}
}

 void AHT_PlayerController::ShowTutorial(const FText& text)
 {
	if (!TutorialWidget)
	{
		if (TutorialWidgetFactory)
		{
			TutorialWidget = CreateWidget<UTutorialUI>(this,TutorialWidgetFactory);
			if (TutorialWidget)
			{
				TutorialWidget->AddToViewport();
				TutorialWidget->SetTutorialText(text);
			}
		}
	}
	
	TutorialWidget->SetVisibility(ESlateVisibility::Visible);
	TutorialWidget->SetTutorialText(text);
 }

 void AHT_PlayerController::HideTutorial()
 {
	if (TutorialWidget)
	{
		TutorialWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	CachedPlayer->PlayerAbilityTags.RemoveTag(CachedPlayer->TutorialTag);
 }

void AHT_PlayerController::OnLeaderboardReceived(const TArray<FLeaderboardEntry>& Entries,int32 myRank, const FLeaderboardEntry& myScore)
{
	if (ScoreLeaderboardUIFactory)
	{
		ScoreLeaderboardUIWidget = CreateWidget<UScoreLeaderboardUI>(this,ScoreLeaderboardUIFactory);
		if (ScoreLeaderboardUIWidget)
		{
			ScoreLeaderboardUIWidget->AddToViewport();
			FOnScoreBtnAction OnScoreBtnAction;
			OnScoreBtnAction.AddLambda([this]()
			{
				FString LevelPath = FString::Printf(TEXT("/Game/HT/Levels/L_Login"));
	
				GetWorld()->SeamlessTravel(LevelPath);
			});
			
			ScoreLeaderboardUIWidget->GenerateMyScore(myScore,myRank,OnScoreBtnAction);
			
			if (Entries.Num() > 0)
			{
				for (int32 i=0;i<Entries.Num();i++)
				{
					bool isMyScore=false;
					if (Entries[i].PlayerName==myScore.PlayerName)
					{
						isMyScore=true;
					}
					ScoreLeaderboardUIWidget->GenerateScoreList(Entries[i],isMyScore,i+1);
					UE_LOG(LogTemp, Log, TEXT("%s | %.2f | %.2f"), *Entries[i].PlayerName, Entries[i].Score, Entries[i].ClearTime);
				}					
			}
			else
			{
				ScoreLeaderboardUIWidget->Text_NoRecord->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

