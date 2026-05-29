#pragma once

#include "CoreMinimal.h"
#include "PJ_MZ_Delegates.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/IHttpRequest.h"
#include "PJ_MZGameMode.generated.h"

UCLASS(abstract)
class APJ_MZGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APJ_MZGameMode();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDynamoDBComponent> DynamoDBComp;

	void OnSubmitComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	
	void SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime);
	
	//void OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	
	UFUNCTION()
	void FetchLeaderboard(FString GameId);
	
};



