#include "ResultUI.h"

#include "MZ_Datas.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "../Framework/PJ_MZGameMode.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Styling/SlateBrush.h"


void UResultUI::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (Button_ShowLeaderboard)
    {
        Button_ShowLeaderboard->OnClicked.RemoveAll(this);
        Button_ShowLeaderboard->OnClicked.AddDynamic(this, &UResultUI::OnClickedButton_ShowLeaderboard);
    }
        
    if (Overlay_Result1&&Overlay_Result2)
    {
        Overlay_Result1->SetVisibility(ESlateVisibility::Visible);
        Overlay_Result2->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UResultUI::SetPhotoImage(int32 index, const FOwningPictureData& pictureData, const FString& timeString, bool bIsDuplicate)
{
    // 중복 여부에 따라 색상 결정
    FSlateColor ScoreColor = (bIsDuplicate || pictureData.FinalScore <= 0.f) ? FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::Green);

    if (pictureData.PhotoImage)
    {
        switch (index)
        {
        case 0:
            if (PhotoImage1 && Text_Score1 && Text_ObjectName1)
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(pictureData.PhotoImage);
                PhotoImage1->SetBrush(Brush);
                Text_Score1->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), pictureData.FinalScore)));
                Text_Score1->SetColorAndOpacity(ScoreColor); // 색상 적용
                Text_ObjectName1->SetText(FText::FromName(pictureData.PicturableDatas.PicturableName != FName() ? pictureData.PicturableDatas.PicturableName : FName(TEXT("None"))));
                Text_ObjectName1->SetColorAndOpacity(ScoreColor); // 색상 적용
            }
            break;

        case 1:
            if (PhotoImage2 && Text_Score2)
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(pictureData.PhotoImage);
                PhotoImage2->SetBrush(Brush);
                Text_Score2->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), pictureData.FinalScore)));
                Text_Score2->SetColorAndOpacity(ScoreColor);
                Text_ObjectName2->SetText(FText::FromName(pictureData.PicturableDatas.PicturableName != FName() ? pictureData.PicturableDatas.PicturableName : FName(TEXT("None"))));
                Text_ObjectName2->SetColorAndOpacity(ScoreColor); // 색상 적용
            }
            break;

        case 2:
            if (PhotoImage3 && Text_Score3)
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(pictureData.PhotoImage);
                PhotoImage3->SetBrush(Brush);
                Text_Score3->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), pictureData.FinalScore)));
                Text_Score3->SetColorAndOpacity(ScoreColor);
                Text_ObjectName3->SetText(FText::FromName(pictureData.PicturableDatas.PicturableName != FName() ? pictureData.PicturableDatas.PicturableName : FName(TEXT("None"))));
                Text_ObjectName3->SetColorAndOpacity(ScoreColor);
            }
            break;
        }
    }

    if (Text_PlayTimeResult)
    {
        Text_PlayTimeResult->SetText(FText::FromString(timeString));
    }
}

void UResultUI::SetTotalScoreText(float totalScore)
{
	
	if (Text_ScoreResult)
	{
		FNumberFormattingOptions Options;
		Options.SetMaximumFractionalDigits(2);

	
		Text_ScoreResult->SetText(
				FText::AsNumber(totalScore, &Options));	
	}
	
}

void UResultUI::OnClickedButton_ShowLeaderboard()
{
    // if (Overlay_Result1&&Overlay_Result2)
    // {
    //     Overlay_Result1->SetVisibility(ESlateVisibility::Collapsed);
    //     Overlay_Result2->SetVisibility(ESlateVisibility::Visible);
    // }
    RemoveFromParent();
    
    auto* gameMode= Cast<APJ_MZGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (gameMode)
    {
        gameMode->FetchLeaderboard(UGameplayStatics::GetCurrentLevelName(GetWorld()));    
    }
    
}
