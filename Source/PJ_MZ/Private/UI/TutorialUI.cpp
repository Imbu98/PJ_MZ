#include "Public/UI/TutorialUI.h"
#include "Components/TextBlock.h"

void UTutorialUI::SetTutorialText(const FText& Text)
{
	if (Text_TutorialText)
	{
		Text_TutorialText->SetText(Text);
	}
}
