#include "LoginUI.h"

#include "Character/Player/HT_PlayerState.h"
#include "Components/Button.h"
#include "Components/DynamoDBComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Framework/LoginGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "StageSelectUI.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"

void ULoginUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 버튼 바인딩
	Btn_ShowLoginPW->OnClicked.AddDynamic(this, &ULoginUI::OnClickShowLoginPassword);
	Btn_ShowRegisterPW->OnClicked.AddDynamic(this, &ULoginUI::OnClickShowRegisterPassword);
	
	Btn_ShowRegisterOverlay->OnClicked.AddDynamic(this, &ULoginUI::OnClickShowRegisterOverlay);
	Btn_Back->OnClicked.AddDynamic(this, &ULoginUI::OnClickBack);
	
	Btn_Login->OnClicked.AddDynamic(this, &ULoginUI::OnClickLogin);
	Btn_Register->OnClicked.AddDynamic(this, &ULoginUI::OnClickRegister);
	
	// 비밀번호 기본 숨김
	Input_LoginPW->SetIsPassword(true);
	Input_RegisterPW->SetIsPassword(true);

	// 로그인 Overlay 외 기본 숨김
	RegisterOverlay->SetVisibility(ESlateVisibility::Collapsed);
	StageSelectOverlay->SetVisibility(ESlateVisibility::Collapsed);
	
	
	// DB 델리게이트 바인딩
	ALoginGameMode* GameMode = Cast<ALoginGameMode>(GetOwningPlayer()->GetWorld()->GetAuthGameMode());
	if (GameMode && GameMode->DynamoDBComp)
	{
		GameMode->DynamoDBComp->OnLoginComplete.AddUObject(this, &ULoginUI::OnLoginResult);
		GameMode->DynamoDBComp->OnRegisterComplete.AddUObject(this, &ULoginUI::OnRegisterResult);
	}
	
	// DT에서 스테이지 정보 가져오기
	if (DT_StageSelectInfo)
	{
		StageSelectData.Empty();
		
		TArray<FStageSelectData*> AllRows;
		DT_StageSelectInfo->GetAllRows<FStageSelectData>(TEXT("StageSelect"), AllRows);
		
		for (FStageSelectData* Row : AllRows)
		{
			if (Row)
			{
				StageSelectData.Add(*Row);
			}
		}
	}
}

void ULoginUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsScrolling)
	{
		float CurrentOffset = ScrollBox_StageSelect->GetScrollOffset();
		float NewOffset = FMath::FInterpTo(CurrentOffset, TargetScrollOffset, InDeltaTime, ScrollInterpSpeed);

		ScrollBox_StageSelect->SetScrollOffset(NewOffset);

		// 목표에 충분히 가까워지면 정지
		if (FMath::IsNearlyEqual(NewOffset, TargetScrollOffset, 0.5f))
		{
			ScrollBox_StageSelect->SetScrollOffset(TargetScrollOffset);
			bIsScrolling = false;
		}
	}
}

void ULoginUI::OnClickShowLoginPassword()
{
	Input_LoginPW->GetIsPassword()? Input_LoginPW->SetIsPassword(false):Input_LoginPW->SetIsPassword(true);
	
}

void ULoginUI::OnClickShowRegisterOverlay()
{
	RegisterOverlay->SetVisibility(ESlateVisibility::Visible);
}

void ULoginUI::OnClickLogin()
{
	FString Id = Input_LoginID->GetText().ToString();
	FString Pw = Input_LoginPW->GetText().ToString();
	
	// 빈칸 체크
	if (Id.IsEmpty() || Pw.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("회원가입: 빈칸 있음"));
		return;
	}

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode && GameMode->DynamoDBComp)
	{
		GameMode->DynamoDBComp->Login(Id, Pw);
	}
}

void ULoginUI::OnClickShowRegisterPassword()
{
	Input_RegisterPW->GetIsPassword()? Input_RegisterPW->SetIsPassword(false):Input_RegisterPW->SetIsPassword(true);
}

void ULoginUI::OnClickBack()
{
	// 입력값 초기화 후 Overlay 숨김
	Input_RegisterID->SetText(FText::GetEmpty());
	Input_RegisterPW->SetText(FText::GetEmpty());
	Input_RegisterNick->SetText(FText::GetEmpty());
	RegisterOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginUI::OnClickRegister()
{
	FString Id = Input_RegisterID->GetText().ToString();
	FString Pw = Input_RegisterPW->GetText().ToString();
	FString Nick = Input_RegisterNick->GetText().ToString();

	// 빈칸 체크
	if (Id.IsEmpty() || Pw.IsEmpty() || Nick.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("회원가입: 빈칸 있음"));
		return;
	}

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode && GameMode->DynamoDBComp)
	{
		GameMode->DynamoDBComp->Register(Id, Pw, Nick);
	}
}

void ULoginUI::OnLoginResult(bool bSuccess, const FString& UserId, const FString& Nickname,int32 stageFlags)
{
	if (!bSuccess)
	{
		// 실패 시 ID/PW 초기화
		Input_LoginPW->SetText(FText::GetEmpty());
		UE_LOG(LogTemp, Warning, TEXT("로그인 실패"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("로그인 성공: %s / %s"), *UserId, *Nickname);

	// PlayerState에 저장 후 레벨 이동
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AHT_PlayerState* PS = PC->GetPlayerState<AHT_PlayerState>();
		if (PS)
		{
			PS->MZ_PlayerID   = UserId;
			PS->MZ_PlayerName = Nickname;
			PS->MZ_StageFlags = stageFlags;
			StageFlags = stageFlags;
		}
		SetStageSelectOverlay(stageFlags);
	}
}

void ULoginUI::OnRegisterResult(bool bSuccess, const FString& ErrorMessage)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("회원가입 실패: %s"), *ErrorMessage);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("회원가입 성공"));

	// 회원가입 성공 시 입력값 로그인 칸으로 옮기고 Overlay 닫기
	Input_LoginID->SetText(Input_RegisterID->GetText());
	Input_LoginPW->SetText(FText::GetEmpty());

	Input_RegisterID->SetText(FText::GetEmpty());
	Input_RegisterPW->SetText(FText::GetEmpty());
	Input_RegisterNick->SetText(FText::GetEmpty());

	RegisterOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void ULoginUI::SetStageSelectOverlay(const int32 stageFlags)
{
	
	if (LoginOverlay&&RegisterOverlay&&StageSelectOverlay)
	{
		LoginOverlay->SetVisibility(ESlateVisibility::Collapsed);
		RegisterOverlay->SetVisibility(ESlateVisibility::Collapsed);
		StageSelectOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	
	// PlayerState에 저장 후 레벨 이동
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AHT_PlayerState* PS = PC->GetPlayerState<AHT_PlayerState>();
		if (PS)
		{
			if (TextBlock_UserName)
			{
				TextBlock_UserName->SetText(FText::FromString(PS->MZ_PlayerName));
			}
		}
	}
	
	if (ScrollBox_StageSelect)
	{
		ScrollBox_StageSelect->SetScrollBarVisibility(ESlateVisibility::Collapsed);
		ScrollBox_StageSelect->SetConsumeMouseWheel(EConsumeMouseWheel::Never);
		ScrollBox_StageSelect->SetAllowOverscroll(false);
		
		for (int32 i = 0; i < StageSelectData.Num(); i++)
		{
			UStageSelectUI* StageSelectUIWidget = CreateWidget<UStageSelectUI>(this, StageSelectUIFactory);
			if (!StageSelectUIWidget) continue;

			UScrollBoxSlot* Slots = Cast<UScrollBoxSlot>(ScrollBox_StageSelect->AddChild(StageSelectUIWidget));
			if (Slots)
			{
				bool bIsFirst = (i == 0);
				bool bIsLast  = (i == StageSelectData.Num() - 1);

				// 간단하게
				float Left  = 350.f;
				float Right = bIsLast ? 350.f : 0.f;

				Slots->SetPadding(FMargin(Left, 50.f, Right, 50.f));
			}
			
			// i+1번째 스테이지 클리어 여부 확인 (i는 0부터, 스테이지는 1부터)
			// 0번째(첫 스테이지)는 항상 열려있음
			bool bIsLocked = false;
			if (i > 0)
			{
				bIsLocked = (stageFlags & (1 << (i - 1))) == 0;
			}

			StageSelectUIWidget->SetStageSelectInfo(StageSelectData[i],bIsLocked);
			StageSelectUIArray.Add(StageSelectUIWidget);
		}
	}
	
	CurrentIndex = 0;
	UpdateVisibility();
	
	// Geometry 계산 완료 후 스크롤 (한 프레임 대기)
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		ScrollToIndex(CurrentIndex);
	});

	Btn_Next->OnClicked.AddDynamic(this, &ULoginUI::OnClickNext);
	Btn_Prev->OnClicked.AddDynamic(this, &ULoginUI::OnClickPrev);
	Btn_StartGame->OnClicked.AddDynamic(this, &ULoginUI::OnClickStartGame);
	
}

void ULoginUI::OnClickNext()
{
	if (CurrentIndex < StageSelectUIArray.Num() - 1)
	{
		CurrentIndex++;
		ScrollToIndex(CurrentIndex);
		UpdateVisibility();
	}
}

void ULoginUI::OnClickPrev()
{
	if (CurrentIndex > 0)
	{
		CurrentIndex--;
		ScrollToIndex(CurrentIndex);
		UpdateVisibility();
	}
}

void ULoginUI::OnClickStartGame()
{
	if (!StageSelectUIArray.IsValidIndex(CurrentIndex)) return;
	
	 FName stageName = StageSelectUIArray[CurrentIndex]->StageData.MoveLevelName;
	
	 FString LevelPath = FString::Printf(TEXT("/Game/HT/Levels/%s"), *stageName.ToString());
	
	 GetWorld()->SeamlessTravel(LevelPath);
	
}

void ULoginUI::ScrollToIndex(int32 Index)
{
	if (!StageSelectUIArray.IsValidIndex(Index)) return;

	UUserWidget* TargetWidget = StageSelectUIArray[Index];

	// ScrollBox와 타겟 위젯의 절대 위치
	FGeometry ScrollGeo = ScrollBox_StageSelect->GetCachedGeometry();
	FGeometry WidgetGeo = TargetWidget->GetCachedGeometry();

	// ScrollBox 기준 위젯의 상대 위치
	FVector2D WidgetLocalPos = ScrollGeo.AbsoluteToLocal(WidgetGeo.GetAbsolutePosition());

	float ScrollBoxWidth = ScrollGeo.GetLocalSize().X;
	float WidgetWidth    = WidgetGeo.GetLocalSize().X;

	// 위젯 중앙이 ScrollBox 중앙에 오도록
	float TargetOffset = ScrollBox_StageSelect->GetScrollOffset()
					   + WidgetLocalPos.X
					   - (ScrollBoxWidth / 2.f)
					   + (WidgetWidth / 2.f);

	TargetScrollOffset = FMath::Max(0.f, TargetOffset);
	bIsScrolling = true;
}

void ULoginUI::UpdateVisibility()
{
	int32 Total = StageSelectUIArray.Num();

	// 하나면 둘 다 숨김
	if (Total <= 1)
	{
		Btn_Prev->SetVisibility(ESlateVisibility::Hidden);
		Btn_Next->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// 맨 처음이면 이전 숨김
	Btn_Prev->SetVisibility(CurrentIndex == 0 
		? ESlateVisibility::Hidden 
		: ESlateVisibility::Visible);

	// 맨 끝이면 다음 숨김
	Btn_Next->SetVisibility(CurrentIndex == Total - 1 
		? ESlateVisibility::Hidden 
		: ESlateVisibility::Visible);
	
	
	if (CurrentIndex > 0)
	{
		bool bIsLocked = (StageFlags & (1 << (CurrentIndex - 1))) == 0;
		Img_Locked->SetVisibility(bIsLocked ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	else
	{
		// 첫 번째 스테이지는 항상 열림
		Img_Locked->SetVisibility(ESlateVisibility::Collapsed);
	}
}

