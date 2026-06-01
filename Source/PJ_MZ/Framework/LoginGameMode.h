#pragma once

#include "CoreMinimal.h"
#include "PJ_MZ_Delegates.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"
UCLASS()
class PJ_MZ_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	ALoginGameMode();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDynamoDBComponent> DynamoDBComp;

};
