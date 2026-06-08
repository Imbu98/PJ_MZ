#include "DialogueUI.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDialogueUI::InitDialogue(const TArray<FDialogueLine>& InLines, const FOnDialogueFinished& OnFinished)
{
	Lines            = InLines;
	CurrentIndex     = 0;
	OnFinishedAction = OnFinished;

	// 대화 시작 시 등장하는 SpeakerID 전부 한 번에 캐싱
	CachedNPCInfo.Empty();
	for (const FDialogueLine& Line : Lines)
	{
		if (CachedNPCInfo.Contains(Line.SpeakerID)) continue;

		FNPCInfoRow* Info = NPCInfoDataTable->FindRow<FNPCInfoRow>(Line.SpeakerID, TEXT(""));
		if (Info)
			CachedNPCInfo.Add(Line.SpeakerID, *Info);
	}

	ShowCurrentLine();
}

void UDialogueUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Btn_DialogueArea)
	{
		Btn_DialogueArea->OnClicked.AddDynamic(this, &UDialogueUI::OnDialogueClicked);
	}
}

void UDialogueUI::OnNextInput()
{
	OnDialogueClicked();
}

void UDialogueUI::SkipDiaglogue()
{
	// 이미 마지막대사면 똑같이 넘긴걸로 표시
	if (CurrentIndex == Lines.Num() - 1)
	{
		OnDialogueClicked();
	}
	else
	{
		// 마지막 대사로 바로 이동
		CurrentIndex = Lines.Num() - 1;
		ShowCurrentLine();
	}
}

void UDialogueUI::OnDialogueClicked()
{
	CurrentIndex++;

	if (Lines.IsValidIndex(CurrentIndex))
	{
		// 다음 대사 출력
		ShowCurrentLine();
	}
	else
	{
		// 마지막 대사 → 대화 종료
		SetVisibility(ESlateVisibility::Collapsed);

		if (OnFinishedAction.IsBound())
			OnFinishedAction.Broadcast();
	}
}



void UDialogueUI::ShowCurrentLine()
{
	 if (!Lines.IsValidIndex(CurrentIndex)) return;

    const FDialogueLine& Line = Lines[CurrentIndex];

  
	// DT 접근 없이 캐시에서 바로
	if (const FNPCInfoRow* Info = CachedNPCInfo.Find(Line.SpeakerID))
	{
		if (Text_SpeakerName)
			Text_SpeakerName->SetText(Info->SpeakerName);
		if (Img_Portrait && Info->Portrait)
			Img_Portrait->SetBrushFromTexture(Info->Portrait);
	}

    if (Text_Content)
        Text_Content->SetText(Line.Content);
}
