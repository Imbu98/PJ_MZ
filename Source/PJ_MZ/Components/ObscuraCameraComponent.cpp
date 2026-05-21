#include "ObscuraCameraComponent.h"

#include "Mz_GameInstance.h"
#include "PicturableComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/HT_PlayerState.h"
#include "Character/Player/HT_Player.h"
#include "Character/Player/HT_PlayerController.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

UObscuraCameraComponent::UObscuraCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->bCaptureEveryFrame = false; // 촬영할 때만 캡처
	SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR; // HUD 제외
}

void UObscuraCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AHT_Player* Player = Cast<AHT_Player>(GetOwner());
	if (!Player) return;

	AHT_PlayerState* PS = Player->GetPlayerState<AHT_PlayerState>();
	if (!PS) return;
	
	Cached_PS = PS;
        
	UCameraComponent* MainCam = Player->GetFirstPersonCameraComponent();
	if (MainCam)
	{
		SceneCapture->AttachToComponent(MainCam, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SceneCapture->FOVAngle = MainCam->FieldOfView; // FOV 맞추기
	}
	
}

void UObscuraCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (Cached_PS&&Cached_PS->IsObscraCooltime)
		
		Cached_PS->currentObscuraCooltime += DeltaTime;

	if (Cached_PS->currentObscuraCooltime >= Cached_PS->MaxObscuraCooltime)
	{
		Cached_PS->IsObscraCooltime = false;
		Cached_PS->currentObscuraCooltime = 0.f;
			
	}
		
	Cached_PS->OnObscuraCooltimeFinished.Broadcast();
	
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
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::Printf(TEXT("OverFlow...Died")));
		
		SetResultUI();
		
		return;
	}
	
	Cached_PS->SetCurrentCanShotCount(CurrentCanShotCount-1);
	
	
	float FinalScore = GetFinalScore(GetPicturableScore());
	
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::Printf(TEXT("Score:%f"),FinalScore));
	
	AddCurrentScoreToArray(FinalScore);
	
	CapturePhoto();
	
	// AActor* Target = GetPrimaryTarget();
	// if (!Target) return;
	//
	
	//
	// UGameplayStatics::ApplyDamage(
	// 	Target,
	// 	FinalDamage,
	// 	GetOwner()->GetInstigatorController(),
	// 	GetOwner(),
	// 	DamageTypeClass
	// );
	//
	// UE_LOG(LogTemp, Log,
	// 	TEXT("Shutter! Target: %s | Points: %d | Multiplier: %.1fx | Damage: %.1f"),
	// 	*Target->GetName(), ActivePointCount,
	// 	GetDamageMultiplier(), FinalDamage);
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

void UObscuraCameraComponent::SetResultUI()
{
	AHT_Player* Player = Cast<AHT_Player>(GetOwner());
	if (!Player) return;
	
	AHT_PlayerController* Pc=  Cast<AHT_PlayerController>(Player->GetController());
	if (!Pc) return;
	
	Pc->SetResultUI();
}

void UObscuraCameraComponent::AddCurrentScoreToArray(float scoreVal)
{
	if (Cached_PS)
	{
		Cached_PS->CachedScoreArray.Add(scoreVal);
	}
}

float UObscuraCameraComponent::GetScoreArrayValue(int32 index)
{
	if (Cached_PS)
	{
		return Cached_PS->CachedScoreArray[index];
	}
	return 0.f;
}

void UObscuraCameraComponent::CapturePhoto()
{
	if (Cached_PS==nullptr) return;
	
	// 새 RenderTarget 생성
	UTextureRenderTarget2D* NewRT = NewObject<UTextureRenderTarget2D>();
	NewRT->InitAutoFormat(1920, 1080);
	NewRT->UpdateResourceImmediate();

	SceneCapture->TextureTarget = NewRT;
	SceneCapture->CaptureScene(); // 캡처

	
	Cached_PS->PhotoList.Add(NewRT); // 앨범에 저장
	
	// 캡처 후 연결 끊기
	SceneCapture->TextureTarget = nullptr;
}

float UObscuraCameraComponent::GetPicturableScore()
{
	if (MainPhotoActor)
	{
		UPicturableComponent* picturableComp = MainPhotoActor->FindComponentByClass<UPicturableComponent>();
		if (picturableComp)
		{
			return picturableComp->GetScore();
		}
	}
	return 0.f;
}





