#include "LoginPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../UI/LoginUI.h"


ALoginPlayerController::ALoginPlayerController()
{
	
}

void ALoginPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
	
	if (LoginUIFactory)
	{
		LoginUIWidget = CreateWidget<ULoginUI>(this, LoginUIFactory);
		if (LoginUIWidget)
		{
			LoginUIWidget->AddToViewport();
		}
	}
}
