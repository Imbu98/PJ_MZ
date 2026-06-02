#include "PJ_MZGameMode.h"

#include "HttpModule.h"
#include "PJ_MZ.h"
#include "Character/Player/HT_PlayerState.h"
#include "Components/DynamoDBComponent.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"

APJ_MZGameMode::APJ_MZGameMode()
{
	DynamoDBComp= CreateDefaultSubobject<UDynamoDBComponent>(TEXT("DynamoDBComp"));
}


void APJ_MZGameMode::SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime,int32 stageIndex)
{
	if (!DynamoDBComp) return;
	
	DynamoDBComp->SubmitScore(GameId, PlayerId, PlayerName, Score, ClearTime,stageIndex);
}

void APJ_MZGameMode::OnSubmitComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!DynamoDBComp) return;
	
	DynamoDBComp->OnSubmitComplete(Request, Response, bSuccess);
}

// void APJ_MZGameMode::OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
// {
// 	if (!DynamoDBComp) return;
// 	
// 	DynamoDBComp->OnFetchComplete(Request, Response, bSuccess);
// }

void APJ_MZGameMode::FetchLeaderboard(const FString& gameId)
{
	if (!DynamoDBComp) return;

	AHT_PlayerState* ps =  Cast<AHT_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	if (ps)
	{
		DynamoDBComp->FetchLeaderboard(gameId,ps->MZ_PlayerID);		
	}
	
	
}

