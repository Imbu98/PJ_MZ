#include "LoginUI.h"

#include "Character/Player/HT_PlayerState.h"
#include "Components/Button.h"
#include "Components/DynamoDBComponent.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Framework/LoginGameMode.h"
#include "Kismet/GameplayStatics.h"

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

	// 회원가입 Overlay 기본 숨김
	RegisterOverlay->SetVisibility(ESlateVisibility::Collapsed);
	
	// DB 델리게이트 바인딩
	ALoginGameMode* GameMode = Cast<ALoginGameMode>(GetOwningPlayer()->GetWorld()->GetAuthGameMode());
	if (GameMode && GameMode->DynamoDBComp)
	{
		GameMode->DynamoDBComp->OnLoginComplete.AddUObject(this, &ULoginUI::OnLoginResult);
		GameMode->DynamoDBComp->OnRegisterComplete.AddUObject(this, &ULoginUI::OnRegisterResult);
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

void ULoginUI::OnLoginResult(bool bSuccess, const FString& UserId, const FString& Nickname)
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
			PS->MZ_PlayerID  = UserId;
			PS->MZ_PlayerName = Nickname;
		}

		GetWorld()->SeamlessTravel(TEXT("/Game/HT/Levels/L_HT"));
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

