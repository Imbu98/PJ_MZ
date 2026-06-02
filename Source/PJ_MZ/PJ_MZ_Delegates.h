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

// DB 랭킹데이터 Get 성공 델리게이트
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLeaderboardFetched, const TArray<FLeaderboardEntry>&,int32,const FLeaderboardEntry&);

// 로그인 성공 델리게이트
DECLARE_MULTICAST_DELEGATE_FourParams(FOnLoginComplete, bool, const FString&,const FString&,int32);
// 회원가입 성공 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRegisterComplete, bool,const FString&);


