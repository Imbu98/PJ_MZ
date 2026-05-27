// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamoDBComponent.h"

#include "HttpModule.h"
#include "MZ_Datas.h"
#include "PJ_MZ.h"
#include "Interfaces/IHttpResponse.h"


// Sets default values for this component's properties
UDynamoDBComponent::UDynamoDBComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDynamoDBComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UDynamoDBComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDynamoDBComponent::SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(DBUrl + TEXT("/scores"));
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	FString Body = FString::Printf(
		TEXT("{\"game_id\":\"%s\",\"player_id\":\"%s\",\"player_name\":\"%s\",\"score\":%d,\"clear_time\":%.2f}"),
		*GameId, *PlayerId, *PlayerName, Score, ClearTime
	);
	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindUObject(this, &UDynamoDBComponent::OnSubmitComplete);
	Request->ProcessRequest();
}


void UDynamoDBComponent::OnSubmitComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess || !Response.IsValid())
	{
		UE_LOG(LogPJ_MZ, Error, TEXT("서버 응답 없음"));
		return;
	}

	UE_LOG(LogPJ_MZ, Log, TEXT("StatusCode: %d"), Response->GetResponseCode());
	UE_LOG(LogPJ_MZ, Log, TEXT("Response: %s"), *Response->GetContentAsString());
}


void UDynamoDBComponent::FetchLeaderboard(FString GameId)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(DBUrl + TEXT("/leaderboard?game_id=") + GameId);
	FString URL = DBUrl + TEXT("/leaderboard?game_id=") + GameId;
	UE_LOG(LogTemp, Log, TEXT("요청 URL: %s"), *URL);
	Request->SetVerb(TEXT("GET"));
	Request->OnProcessRequestComplete().BindUObject(this, &UDynamoDBComponent::OnFetchComplete);
	Request->ProcessRequest();
}

void UDynamoDBComponent::OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess) return;

	if (!bSuccess || !Response.IsValid()) return;
	
	// 응답 내용 확인
	UE_LOG(LogTemp, Log, TEXT("StatusCode: %d"), Response->GetResponseCode());
	UE_LOG(LogTemp, Log, TEXT("Response: %s"), *Response->GetContentAsString());

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		TArray<FLeaderboardEntry> Entries;

		for (auto& Item : JsonArray)
		{
			TSharedPtr<FJsonObject> Obj = Item->AsObject();

			FLeaderboardEntry Entry;
			Entry.PlayerName = Obj->GetStringField(TEXT("player_name"));
			Entry.Score      = Obj->GetIntegerField(TEXT("score"));
			Entry.ClearTime  = Obj->GetNumberField(TEXT("clear_time"));

			Entries.Add(Entry);
		}

		// 델리게이트 호출 → 바인딩된 모든 곳에 전달
		OnLeaderboardFetched.Broadcast(Entries);
	}
}

