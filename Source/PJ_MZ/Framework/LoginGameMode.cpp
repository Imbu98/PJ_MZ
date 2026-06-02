#include "LoginGameMode.h"

#include "Character/Player/HT_PlayerState.h"
#include "Components/DynamoDBComponent.h"
#include "Kismet/GameplayStatics.h"


ALoginGameMode::ALoginGameMode()
{
	DynamoDBComp = CreateDefaultSubobject<UDynamoDBComponent>("DynamoDBComp");
	
	bUseSeamlessTravel = true;
}

void ALoginGameMode::FetchLeaderboard(const FString& gameId)
{
	if (!DynamoDBComp) return;

	AHT_PlayerState* ps =  Cast<AHT_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	if (ps)
	{
		DynamoDBComp->FetchLeaderboard(gameId,ps->MZ_PlayerID);		
	}
}

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	
}

