#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Mz_GameInstance.generated.h"

UCLASS()
class PJ_MZ_API UMz_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;
	
public:
	bool bIsShotCountInitialized=false;
	int CachedShotCount;
};
