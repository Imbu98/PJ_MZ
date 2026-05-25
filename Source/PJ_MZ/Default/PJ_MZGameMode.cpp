#include "PJ_MZGameMode.h"

#include "HttpModule.h"
#include "PJ_MZ.h"
#include "Interfaces/IHttpResponse.h"

APJ_MZGameMode::APJ_MZGameMode()
{
	
}


void APJ_MZGameMode::SubmitScore(FString GameId, FString PlayerId, FString PlayerName, int32 Score, float ClearTime)
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
	Request->OnProcessRequestComplete().BindUObject(this, &APJ_MZGameMode::OnSubmitComplete);
	Request->ProcessRequest();
}

void APJ_MZGameMode::OnSubmitComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess || !Response.IsValid())
	{
		UE_LOG(LogPJ_MZ, Error, TEXT("서버 응답 없음"));
		return;
	}

	UE_LOG(LogPJ_MZ, Log, TEXT("StatusCode: %d"), Response->GetResponseCode());
	UE_LOG(LogPJ_MZ, Log, TEXT("Response: %s"), *Response->GetContentAsString());
}

void APJ_MZGameMode::OnFetchComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (!bSuccess) return;

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    
	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		for (int32 i = 0; i < JsonArray.Num(); i++)
		{
			TSharedPtr<FJsonObject> Obj = JsonArray[i]->AsObject();
			FString Name = Obj->GetStringField(TEXT("PlayerName"));
			int32 Score = Obj->GetIntegerField(TEXT("Score"));
			float ClearTime = Obj->GetNumberField(TEXT("clear_time"));

			UE_LOG(LogTemp, Log, TEXT("#%d %s | 점수: %d | 시간: %.2f초"), 
				i + 1, *Name, Score, ClearTime);
		}
	}
}
