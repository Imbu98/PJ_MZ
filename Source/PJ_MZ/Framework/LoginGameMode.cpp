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

