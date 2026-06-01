#include "LoginGameMode.h"
#include "Components/DynamoDBComponent.h"

ALoginGameMode::ALoginGameMode()
{
	DynamoDBComp = CreateDefaultSubobject<UDynamoDBComponent>("DynamoDBComp");
	
	bUseSeamlessTravel = true;
}

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void ALoginGameMode::Login(FString Id, FString Password)
{
	
}

void ALoginGameMode::Register(FString Id, FString Password, FString Nickname)
{
	
}
