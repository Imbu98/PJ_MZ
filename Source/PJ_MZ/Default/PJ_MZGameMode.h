#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/IHttpRequest.h"
#include "PJ_MZGameMode.generated.h"

UCLASS(abstract)
class APJ_MZGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APJ_MZGameMode();

	void OnSubmitComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	void SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime);

	void OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	UPROPERTY()
	FString DBUrl = "https://gj2xbjow98.execute-api.ap-northeast-2.amazonaws.com/Prod";
	
	
};



