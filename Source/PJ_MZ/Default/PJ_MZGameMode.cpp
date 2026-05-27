#include "PJ_MZGameMode.h"

#include "HttpModule.h"
#include "PJ_MZ.h"
#include "Components/DynamoDBComponent.h"
#include "Interfaces/IHttpResponse.h"

APJ_MZGameMode::APJ_MZGameMode()
{
	DynamoDBComp= CreateDefaultSubobject<UDynamoDBComponent>(TEXT("DynamoDBComp"));
}


void APJ_MZGameMode::SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime)
{
	if (!DynamoDBComp) return;
	
	DynamoDBComp->SubmitScore(GameId, PlayerId, PlayerName, Score, ClearTime);
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

void APJ_MZGameMode::FetchLeaderboard(FString GameId)
{
	if (!DynamoDBComp) return;
	
	DynamoDBComp->FetchLeaderboard(GameId);
}

