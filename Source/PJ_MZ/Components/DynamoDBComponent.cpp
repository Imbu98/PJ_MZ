#include "DynamoDBComponent.h"

#include "HttpModule.h"
#include "MZ_Datas.h"
#include "PJ_MZ.h"
#include "GameFramework/GameModeBase.h"
#include "../Character/Player/HT_PlayerState.h"
#include "Interfaces/IHttpResponse.h"


class AHT_PlayerState;
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

void UDynamoDBComponent::SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime,int32 stageIndex)
{
	
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(DBUrl + TEXT("/scores"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString Body = FString::Printf(
		TEXT("{\"game_id\":\"%s\",\"player_id\":\"%s\",\"player_name\":\"%s\",\"score\":%d,\"clear_time\":%.2f,\"stage_index\":%d}"),
		*GameId, *PlayerId, *PlayerName, Score, ClearTime, stageIndex
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
}


void UDynamoDBComponent::FetchLeaderboard(const FString& GameId,const FString& PlayerId)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	FString URL = DBUrl + TEXT("/leaderboard?game_id=") + GameId + TEXT("&player_id=") + PlayerId;
	Request->SetURL(URL);
	UE_LOG(LogTemp, Log, TEXT("요청 URL: %s"), *URL);
	Request->SetVerb(TEXT("GET"));
	Request->OnProcessRequestComplete().BindUObject(this, &UDynamoDBComponent::OnFetchComplete);
	Request->ProcessRequest();
}

void UDynamoDBComponent::OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess || !Response.IsValid()) return;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		// Top10 파싱
		TArray<FLeaderboardEntry> Top10;
		TArray<TSharedPtr<FJsonValue>> Top10Array = JsonObject->GetArrayField(TEXT("top10"));
		for (auto& Item : Top10Array)
		{
			TSharedPtr<FJsonObject> Obj = Item->AsObject();
			FLeaderboardEntry Entry;
			Entry.PlayerName = Obj->GetStringField(TEXT("player_name"));
			Entry.Score      = Obj->GetIntegerField(TEXT("score"));
			Entry.ClearTime  = Obj->GetNumberField(TEXT("clear_time"));
			Top10.Add(Entry);
		}

		// 내 순위
		int32 MyRank = (int32)JsonObject->GetNumberField(TEXT("my_rank"));

		// 내 데이터
		FLeaderboardEntry MyData;
		TSharedPtr<FJsonObject> MyDataObj = JsonObject->GetObjectField(TEXT("my_data"));
		if (MyDataObj.IsValid())
		{
			MyData.PlayerName = MyDataObj->GetStringField(TEXT("player_name"));
			MyData.Score      = MyDataObj->GetIntegerField(TEXT("score"));
			MyData.ClearTime  = MyDataObj->GetNumberField(TEXT("clear_time"));
		}

		OnLeaderboardFetched.Broadcast(Top10, MyRank, MyData);
	}
}

void UDynamoDBComponent::Login(FString UserId, FString Password)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(DBUrl + TEXT("/login"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString Body = FString::Printf(
		TEXT("{\"user_id\":\"%s\",\"password\":\"%s\"}"),
		*UserId, *Password
	);

	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindUObject(this, &UDynamoDBComponent::OnLoginComplete_Internal);
	Request->ProcessRequest();
}

void UDynamoDBComponent::OnLoginComplete_Internal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess || !Response.IsValid())
	{
		OnLoginComplete.Broadcast(false, TEXT(""), TEXT(""), 0);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString Message = JsonObject->GetStringField(TEXT("message"));
		if (Message == TEXT("SUCCESS"))
		{
			FString UserId = JsonObject->GetStringField(TEXT("user_id"));
			FString Nickname = JsonObject->GetStringField(TEXT("nickname"));
			int32 StageFlags = (int32)JsonObject->GetNumberField(TEXT("stage_flags"));
			OnLoginComplete.Broadcast(true, UserId, Nickname, StageFlags);
		}
		else
		{
			OnLoginComplete.Broadcast(false, TEXT(""), TEXT(""), 0);
		}
	}
}

void UDynamoDBComponent::Register(FString UserId, FString Password, FString Nickname)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(DBUrl + TEXT("/register"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	FString Body = FString::Printf(
		TEXT("{\"user_id\":\"%s\",\"password\":\"%s\",\"nickname\":\"%s\"}"),
		*UserId, *Password, *Nickname
	);

	Request->SetContentAsString(Body);
	Request->OnProcessRequestComplete().BindUObject(this, &UDynamoDBComponent::OnRegisterComplete_Internal);
	Request->ProcessRequest();
}



void UDynamoDBComponent::OnRegisterComplete_Internal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess || !Response.IsValid())
	{
		OnRegisterComplete.Broadcast(false, TEXT("서버 오류"));
		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString Message = JsonObject->GetStringField(TEXT("message"));
		OnRegisterComplete.Broadcast(StatusCode == 200, Message);
	}
}

