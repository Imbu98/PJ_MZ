#pragma once

#include "MZ_Datas.h"
#include "CoreMinimal.h"

// 정신력 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FMentalityChangedDelegate,float);
// 스테미나 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FStaminaChangeDelegate,float);
// 촬영횟수 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FShotCountChangeDelegate,int32);
// 쿨타임 델리게이트
DECLARE_MULTICAST_DELEGATE(FObscuraCooltimeFinished);
// 던전 생성 완료 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnDungeonGenerationComplete);
// 카메라 줌인 , 줌아웃 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FCameraFOVChangeDelegate,float);

// 델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLeaderboardFetched, const TArray<FLeaderboardEntry>&);


