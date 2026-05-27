// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJ_MZ_Delegates.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "DynamoDBComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PJ_MZ_API UDynamoDBComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDynamoDBComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	FOnLeaderboardFetched OnLeaderboardFetched;

public:
	void FetchLeaderboard(FString GameId); 
	
	void OnSubmitComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	void SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime);

	void OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	UPROPERTY()
	FString DBUrl = "https://gj2xbjow98.execute-api.ap-northeast-2.amazonaws.com/Prod";
};
