#include "StageNPC.h"

#include "Mz_GameInstance.h"
#include "Character/Player/HT_Player.h"
#include "Character/Player/HT_PlayerController.h"
#include "Character/Player/HT_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../../Components/ObscuraCameraComponent.h"
#include "UI/DialogueUI.h"
#include "../../MZ_Datas.h"


AStageNPC::AStageNPC()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStageNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStageNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStageNPC::Interacted_Implementation()
{
	Super::Interacted_Implementation();

	if (!CachedPlayer) return;
	
	AHT_PlayerState* PS = CachedPlayer->GetPlayerState<AHT_PlayerState>();

	if (!PS) return;
	
	if (!PS->bHasSpokenToNPC)
	{
		HandleFirstTalk(PS);				
	}
	else 
	{
		HandleReturnTalk(PS);				
	}
		
}

void AStageNPC::GivePlayerToCamera()
{
	// 촬영 횟수 초기화 → PlayerState에서
	if (!CachedPlayer) return;

	AHT_PlayerState* PS = CachedPlayer->GetPlayerState<AHT_PlayerState>();
	if(!PS) return;

	AHT_PlayerController* PC = Cast<AHT_PlayerController>(CachedPlayer->GetController());
	if (!PC) return;

	// 카메라 능력 주기
	FGameplayTag ObscuraTag = CachedPlayer->ObscuraTag;
	if (!CachedPlayer->PlayerAbilityTags.HasTag(ObscuraTag))
	{
		CachedPlayer->PlayerAbilityTags.AddTag(ObscuraTag);	
	}
	
		UMz_GameInstance* Mz_GI = Cast<UMz_GameInstance>(GetGameInstance());
		if (Mz_GI)
		{
			// 처음초기화하는거면 PS에서 Init
			if (!Mz_GI->bIsShotCountInitialized)
			{
				PS->InitShotCount();
				PS->OnShotCountChangeDelegate.Broadcast(PS->GetCurrentCanShotCount());
			}
			// 이미 한번 초기화해서 게임인스턴스에 값이 있으면 그 값으로 초기화 ( 근데 게임이 끝나고 다른 레벨을 시작할 때 그 값이 남아 있을 수 있음, 초기화 필요 )
			else
			{
				PS->CurrentCanShotCount = Mz_GI->CachedShotCount;
			}
			Mz_GI->bIsShotCountInitialized = true;
		}
	// 대화창 숨기기
	PC->HideDialogue();
}

void AStageNPC::HandleFirstTalk(AHT_PlayerState* PS)
{
	PS->bHasSpokenToNPC    = true;
	PS->CurrentCanShotCount = PS->MaxCanShotCount;

	AHT_PlayerController* PC = Cast<AHT_PlayerController>(CachedPlayer->GetController());
	if (!PC || !DialogueDataTable) return;

	FDialogueRow* Row = DialogueDataTable->FindRow<FDialogueRow>(FirstTalkRowName, TEXT(""));
	if (!Row) return;

	FOnDialogueFinished FinishedDelegate;
	FinishedDelegate.AddDynamic(this, &AStageNPC::GivePlayerToCamera);

	PC->ShowDialogue(Row->Lines, FinishedDelegate);
	
}

void AStageNPC::HandleReturnTalk(AHT_PlayerState* PS)
{
	if (PS->CurrentCanShotCount > 0)
	{
		// 사진 횟수가 남아있으면 선택지 표시
		ShowChoiceWidget();
	}
	else
	{
		// 횟수 소진 → 게임 끝
		if (CachedPlayer)
		{
			UObscuraCameraComponent* ObscuraComp =  CachedPlayer->GetComponentByClass<UObscuraCameraComponent>();
			if (ObscuraComp)
			{
				ObscuraComp->OnGameEnd();				
			}
		}
	}
}

void AStageNPC::ShowChoiceWidget()
{
	if (!CachedPlayer) return;
	
	AHT_PlayerController* PC = Cast<AHT_PlayerController>(CachedPlayer->GetController());
	if (!PC) return;
	
	 PC->ShowPopup(FText::FromString(TEXT("사진 횟수가 남아있습니다.\n정말 나가시겠습니까?")),
	 	FText::FromString(TEXT("나가기")),
	 	FText::FromString(TEXT("남아있기")),
	 	ConfirmDelegate,
	 	CancelDelegate);
}

void AStageNPC::OnConfirmExit()
{
	// 나가기 확인
	if (CachedPlayer)
	{
		UObscuraCameraComponent* ObscuraComp =  CachedPlayer->GetComponentByClass<UObscuraCameraComponent>();
		if (ObscuraComp)
		{
			ObscuraComp->OnGameEnd();				
		}
	}
}

void AStageNPC::OnCancelExit()
{
	AHT_PlayerController* PC = Cast<AHT_PlayerController>(CachedPlayer->GetController());
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}



