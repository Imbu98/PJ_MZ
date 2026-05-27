#include "ResultUI.h"

#include "MZ_Datas.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Styling/SlateBrush.h"


void UResultUI::SetPhotoImage(int32 index, const FOwningPictureData& pictureData,const FString& timeString)
{
	if (pictureData.PhotoImage)
	{
		switch (index)
		{
		case 0: 
			if (PhotoImage1&&Text_Score1&&Text_ObjectName1)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(pictureData.PhotoImage);
				PhotoImage1->SetBrush(Brush);
				
				Text_Score1->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), pictureData.FinalScore)));
				Text_ObjectName1->SetText(FText::FromName(pictureData.PicturableDatas.PicturableName!=
					FName()?pictureData.PicturableDatas.PicturableName:FName(TEXT("None"))));
			}break;
	
		case 1:
			
			if (PhotoImage2&&Text_Score2)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(pictureData.PhotoImage);
				PhotoImage2->SetBrush(Brush);
				
				Text_Score2->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), pictureData.FinalScore)));
				Text_ObjectName2->SetText(FText::FromName(pictureData.PicturableDatas.PicturableName!=
					FName()?pictureData.PicturableDatas.PicturableName:FName(TEXT("None"))));
			}break;
			case 2:
			if (PhotoImage3&&Text_Score3)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(pictureData.PhotoImage);
				PhotoImage3->SetBrush(Brush);
				
				Text_Score3->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), pictureData.FinalScore)));
				Text_ObjectName3->SetText(FText::FromName(pictureData.PicturableDatas.PicturableName!=
					FName()?pictureData.PicturableDatas.PicturableName:FName(TEXT("None"))));
			}break;
		default:
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
