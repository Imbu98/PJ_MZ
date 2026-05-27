#include "ResultUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Styling/SlateBrush.h"


void UResultUI::SetPhotoImage(int32 index, UTexture2D* renderTarget,float scoreVal,const FString Formattedstring)
{
	if (renderTarget)
	{
		switch (index)
		{
		case 0: 
			if (PhotoImage1&&ScoreText1)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(renderTarget);
				PhotoImage1->SetBrush(Brush);
				
				ScoreText1->SetText(FText::FromString(FString::Printf(TEXT("%f"), scoreVal)));
			}break;
	
		case 1:
			
			if (PhotoImage2&&ScoreText2)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(renderTarget);
				PhotoImage2->SetBrush(Brush);
				
				ScoreText2->SetText(FText::FromString(FString::Printf(TEXT("%f"), scoreVal)));
			}break;
			case 2:
			if (PhotoImage3&&ScoreText3)
			{
				FSlateBrush Brush;
				Brush.SetResourceObject(renderTarget);
				PhotoImage3->SetBrush(Brush);
				
				ScoreText3->SetText(FText::FromString(FString::Printf(TEXT("%f"), scoreVal)));
			}break;
		default:
			break;
		}
	}
	
	if (Text_PlayTimeResult)
	{
Text_PlayTimeResult->SetText(FText::FromString(Formattedstring));		
	}
}
