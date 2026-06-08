#include "ObscuraCameraComponent.h"

#include "Mz_GameInstance.h"
#include "PicturableComponent.h"
#include "PJ_MZ.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/HT_PlayerState.h"
#include "Character/Player/HT_Player.h"
#include "Character/Player/HT_PlayerController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "../Framework/PJ_MZGameMode.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"

UObscuraCameraComponent::UObscuraCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UObscuraCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AHT_Player* Player = Cast<AHT_Player>(GetOwner());
	if (!Player) return;

	AHT_PlayerState* PS = Player->GetPlayerState<AHT_PlayerState>();
	if (!PS) return;
	
	Cached_PS = PS;
	
}

void UObscuraCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 쿨타임 처리
	if (Cached_PS&&Cached_PS->IsObscraCooltime)
		
		Cached_PS->currentObscuraCooltime += DeltaTime;

	if (Cached_PS->currentObscuraCooltime >= Cached_PS->MaxObscuraCooltime)
	{
		Cached_PS->IsObscraCooltime = false;
		Cached_PS->currentObscuraCooltime = 0.f;
			
	}
	Cached_PS->OnObscuraCooltimeFinished.Broadcast();

	// FOV Lerp
	if (CurrentMode == EObscuraModeAction::CAMERAMODE)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomInterpSpeed);

		AHT_Player* Player = Cast<AHT_Player>(GetOwner());
		if (Player)
		{
			UCameraComponent* Cam = Player->GetFirstPersonCameraComponent();
			if (Cam)
			{
				Cam->SetFieldOfView(CurrentFOV);
			}
		}
	}
	
}



void UObscuraCameraComponent::InitPoints(int32 PointCount)
{
	PointActiveList.Init(false, PointCount);
	PointHitActors.Init(nullptr, PointCount);
	ActivePointCount = 0;
}


void UObscuraCameraComponent::SetPointActive(int32 Index, bool bActive, AActor* HitActor)
{
	if (!PointActiveList.IsValidIndex(Index)) return;

	bool bWas = PointActiveList[Index];
	PointActiveList[Index] = bActive;
	PointHitActors[Index] = HitActor;

	// 카운트 갱신
	if (bActive && !bWas)       ActivePointCount++;
	else if (!bActive && bWas)  ActivePointCount--;
}

void UObscuraCameraComponent::ApplyShutterDamage()
{
	if (Cached_PS==nullptr) return;
	
	int32 CurrentCanShotCount =Cached_PS->GetCurrentCanShotCount();
	// 촬영횟수가 없으면 사망
	if (CurrentCanShotCount <= 0)
	{
		float penaltyAmount = -10.f;
		AHT_Player* Player = Cast<AHT_Player>(GetOwner());
		if (Player)
		{
			Player->OnChangeMentality(penaltyAmount);
		}
		
		
		// GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::Printf(TEXT("OverFlow...Died")));
		//
		// OnGameEnd();
		
		return;
	}
	
	if (MainPhotoActor)
	{
		UPicturableComponent* picturableComp = MainPhotoActor->FindComponentByClass<UPicturableComponent>();
		if (picturableComp)
		{
			picturableComp->PictureTaken();
		}
	}
	
	Cached_PS->SetCurrentCanShotCount(CurrentCanShotCount-1);
	
	FOwningPictureData PictureData;
	PictureData.PicturableDatas = GetPicturableData();
	PictureData.FinalScore = GetFinalScore(PictureData.PicturableDatas.PicturableMaxScore);
	PictureData.PhotoImage=nullptr;
	
	CapturePhoto(PictureData);
	
	AddCurrentDataToArray(PictureData);
	
}

float UObscuraCameraComponent::GetFinalScore(float score) const
{
	if (ActivePointCount <= 0) return 0.0f;

	if (ActivePointCount >= 5) return score / 1.0f;
	if (ActivePointCount >= 3) return score / 2.0f;
	if (ActivePointCount >= 1) return score / 3.0f;
	
	return 0.0f;
}


bool UObscuraCameraComponent::ObscuraCanShot()
{
	return Cached_PS->IsObscraCooltime;
}

float UObscuraCameraComponent::GetObscuraCooltimePercent()
{
	return Cached_PS->currentObscuraCooltime/Cached_PS->MaxObscuraCooltime;
}

void UObscuraCameraComponent::SetResultUI(const float totalScore)
{
	AHT_Player* Player = Cast<AHT_Player>(GetOwner());
	if (!Player) return;
	
	AHT_PlayerController* Pc=  Cast<AHT_PlayerController>(Player->GetController());
	if (!Pc) return;
	
	Pc->SetResultUI(totalScore);
}

void UObscuraCameraComponent::OnGameEnd()
{
	APJ_MZGameMode* GameMode = Cast<APJ_MZGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode를 찾을 수 없음"));
		return;
	}

	if (!Cached_PS) return;
	

	// 임시 플레이어 정보 - 나중에 교체
	FString GameId    = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FString PlayerId  = Cached_PS->MZ_PlayerID;
	FString PlayerName = Cached_PS->MZ_PlayerName;
	
	// 총 점수 계산
	float totalScore = CalculateTotalScore();
	
	// 세트 보너스 적용
	totalScore = CalculateSetBonus(totalScore);
	
	// 끝나는데 걸린 시간
	float endTime = Cached_PS->GetElapsedSeconds();

	//Clear처리할 스테이지 index
	int32 stageIndex = CheckIsUnLocked(totalScore);
	
	if (stageIndex >= 1)
	{
		Cached_PS->MZ_StageFlags |= (1 << (stageIndex - 1));
	}
	
	GameMode->SubmitScore(GameId, PlayerId, PlayerName, totalScore, endTime,stageIndex);
	
	SetResultUI(totalScore);
	
	
}

void UObscuraCameraComponent::AddCurrentDataToArray(FOwningPictureData datas)
{
	if (!Cached_PS) return;

	FName NewName = datas.PicturableDatas.PicturableName;

	for (FOwningPictureData& Existing : Cached_PS->OwningPictureArray)
	{
		if (Existing.PicturableDatas.PicturableName != NewName) continue;

		// 같은 이름 발견 → 점수 비교
		if (datas.FinalScore > Existing.FinalScore)
		{
			// 새거가 더 높으면 기존꺼를 중복으로
			Existing.IsDuplicate = true;
		}
		else
		{
			// 기존꺼가 더 높으면 새거를 중복으로
			datas.IsDuplicate = true;
		}
	}
	Cached_PS->OwningPictureArray.Add(datas);
}

FOwningPictureData UObscuraCameraComponent::GetOwningPictureDataInArray(int32 index)
{
	if (Cached_PS)
	{
		return Cached_PS->OwningPictureArray[index];
	}
	return FOwningPictureData();
}

void UObscuraCameraComponent::CapturePhoto(FOwningPictureData& datas)
{
    AHT_PlayerController* PC = Cast<AHT_PlayerController>(GetWorld()->GetFirstPlayerController());
    if (!PC) return;

    // // 한 프레임 뒤 캡처
    // GetWorld()->GetTimerManager().SetTimerForNextTick([this, PC]()
    // {
        // 1. 유효성 검사
        ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
        if (!LocalPlayer || !LocalPlayer->ViewportClient || !LocalPlayer->ViewportClient->Viewport) return;

        FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;

        TArray<FColor> Pixels;
        
        
        if (!Viewport->ReadPixels(Pixels))
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Viewport ReadPixels 실패! (화면을 읽어오지 못함)"));
            return;
        }

        int32 Width = Viewport->GetSizeXY().X;
        int32 Height = Viewport->GetSizeXY().Y;

        // 💡 [핵심 1] 뷰포트에서 읽어온 픽셀은 Alpha가 0일 확률이 매우 높습니다. 
        // UI에서 투명하게 보이는 것을 막기 위해 강제로 불투명(255)하게 만듭니다.
        for (FColor& Pixel : Pixels)
        {
            Pixel.A = 255;
        }

        // 2. 가벼운 UTexture2D 생성 (CreateTransient가 이미 필요한 메모리를 할당함)
        UTexture2D* Photo = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
        if (!Photo) return;

        Photo->SRGB = true;
        // Photo->AddToRoot(); // ⚠️ [주의] AddToRoot는 메모리 누수를 유발할 수 있습니다!
        // Cached_PS->PhotoList에 UPROPERTY() 매크로가 있다면 굳이 AddToRoot()를 안 써도 가비지 컬렉션(GC)에 날아가지 않습니다.

        // 3. 💡 [핵심 2] Realloc을 지우고, 할당된 메모리에 안전하게 덮어쓰기만 합니다.
        void* TextureData = Photo->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
        Photo->GetPlatformData()->Mips[0].BulkData.Unlock();

        // 강제로 GPU에 올리기
        Photo->UpdateResource();

        // 4. 안전하게 배열에 저장
        // (타이머(람다) 안에서 실행되므로, 그 1프레임 사이에 플레이어가 죽거나 PS가 파괴됐을 수 있어 체크 필요)
        if (Cached_PS)
        {
        	datas.PhotoImage = Photo;
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
                FString::Printf(TEXT("찰칵! 앨범 저장 완료 W:%d H:%d"), Width, Height));
        }
    // });
}


FPicturableDatas UObscuraCameraComponent::GetPicturableData()
{
	if (MainPhotoActor)
	{
		UPicturableComponent* picturableComp = MainPhotoActor->FindComponentByClass<UPicturableComponent>();
		if (picturableComp)
		{
			return picturableComp->GetDatas();
		}
	}
	return FPicturableDatas();
}

float UObscuraCameraComponent::CalculateTotalScore()
{
	float TotalScore = 0.f;
	
	for (const FOwningPictureData& Data : Cached_PS->OwningPictureArray)
	{
		if (!Data.IsDuplicate)
			TotalScore += Data.FinalScore;
	}
	return TotalScore;
}

float UObscuraCameraComponent::CalculateSetBonus(float totalScore)
{
	TMap<FName, int32> SetIDCountMap;

	for (const FOwningPictureData& owningData : Cached_PS->OwningPictureArray)
	{
		// 중복 제외
		if (owningData.IsDuplicate) continue;

		SetIDCountMap.FindOrAdd(owningData.PicturableDatas.SetID)++;
	}

	if (DT_SetBonusInfo)
	{
		for (const auto& Pair : SetIDCountMap)
		{
			if (FSetBonus* SetBonus = DT_SetBonusInfo->FindRow<FSetBonus>(Pair.Key, TEXT("SetBonusDT Missed")))
			{
				if (Pair.Value >= 3)
					totalScore += SetBonus->Set3BonusScore;
				else if (Pair.Value >= 2)
					totalScore += SetBonus->Set2BonusScore;
			}
		}
	}
	return totalScore;
}

// Dt에 적용된 해금점수보다 현재 점수가 높은지 판단
int32 UObscuraCameraComponent::CheckIsUnLocked(float score)
{
	FName levelName = *UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	if (!DT_StageSelectData)
	{
		UE_LOG(LogPJ_MZ,Warning,TEXT("No DT_StageSelectData"));
		return 0;
	}
	// 모든 행 이름 가져오기
	TArray<FName> RowNames = DT_StageSelectData->GetRowNames();

	// 현재 레벨이 몇 번째 행인지 찾기
	int32 StageIndex = RowNames.IndexOfByKey(levelName) + 1; // 1부터 시작
	
	FStageSelectData* StageSelectData = DT_StageSelectData->FindRow<FStageSelectData>(levelName,TEXT("No StageSelectData"));
	if (StageSelectData)
	{
		if (score>=StageSelectData->UnLockScore)
		{
			return StageIndex;
		}
	}
	return 0;
}





