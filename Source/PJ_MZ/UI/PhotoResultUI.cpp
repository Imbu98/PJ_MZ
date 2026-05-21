#include "PhotoResultUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Styling/SlateBrush.h"


void UPhotoResultUI::SetPhotoImage(int32 index, UTextureRenderTarget2D* renderTarget,float scoreVal)
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
}
