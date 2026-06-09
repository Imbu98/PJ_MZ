#include "ObscuraUI.h"

#include "PJ_MZ.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Character/Player/HT_Player.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ObscuraCameraComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/Player/HT_PlayerState.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PicturableComponent.h"


void UObscuraUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	PC = GetOwningPlayer();

	// 위젯 트리에서 UImage 전부 수집
	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UImage* Img = Cast<UImage>(Widget))
		{
			// 태그로 포인트만 필터링
			if (Img->GetFName().ToString().Contains("Point"))
			{
				FinderPoints.Add(Img);
			}
		}
	});

	if (CameraObscuraComp)
		CameraObscuraComp->InitPoints(FinderPoints.Num());

	ResetObscura();
	
	BindingEvent();
}

// CameraObscuraWidget.cpp
void UObscuraUI::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	
	
	// 감지 비활성화 상태면 스킵
	if (!bDetectionActive || !PC || !CameraObscuraComp) return;
	UpdateAllPoints();
	if (CameraObscuraComp)
	{
		if (CameraObscuraComp->ObscuraCanShot())
		{
			ObscuraCooltimeUpdate();
		}	
	}
}

void UObscuraUI::BindingEvent()
{
	// 기존 CameraObscuraComp 대신 PlayerState에서 읽기
	AHT_PlayerState* PS = GetOwningPlayer()->GetPlayerState<AHT_PlayerState>();
	if (PS)
	{
		if (PS)
		{
			PS->OnShotCountChangeDelegate.RemoveAll(this);
			PS->OnShotCountChangeDelegate.AddUObject(this, &UObscuraUI::OnShotCountUpdated);
			
			PS->OnMentalityChangeDelegate.RemoveAll(this);
			PS->OnMentalityChangeDelegate.AddUObject(this, &UObscuraUI::OnPlayerMentalityBarUpdated);
			
			PS->OnObscuraCooltimeFinished.RemoveAll(this);
			PS->OnObscuraCooltimeFinished.AddUObject(this,&UObscuraUI::OnObscuraBarReset);
			
			PS->OnStaminaBarUpdated.RemoveAll(this);
			PS->OnStaminaBarUpdated.AddUObject(this, &UObscuraUI::OnStaminaBarUpdated);
			
			PS->OnCameraFOVChangeDelegate.RemoveAll(this);
			PS->OnCameraFOVChangeDelegate.AddUObject(this, &UObscuraUI::OnCameraFOVUpdated);
		
		}
	
	}
	
}

void UObscuraUI::UpdateAllPoints()
{
    // 1단계: 모든 포인트 트레이스 결과 캐싱
    TArray<TPair<bool, AActor*>> TraceResults;
    TSet<AActor*> UniqueActors;

    for (UImage* Point : FinderPoints)
    {
        AActor* HitActor = nullptr;
        bool bHit = Point ? TraceFromScreenPoint(Point, HitActor) : false;
    	
        TraceResults.Add(TPair<bool, AActor*>(bHit, HitActor));
        if (bHit && HitActor) UniqueActors.Add(HitActor);
    }

    // 2단계: 잡힌 액터가 2개 이상이면 0번 포인트에 가장 가까운 액터를 Main으로
	// 2단계: Main Photo Actor 선정 로직 간소화
	AActor* FinalMainActor = nullptr;

	if (UniqueActors.Num() == 1)
	{
		FinalMainActor = *UniqueActors.begin(); // 1개면 바로 확정
	}
	else if (UniqueActors.Num() >= 2)
	{
		FVector2D CenterScreenPos = GetPointScreenCenter(FinderPoints[0]);
		float MinDist = FLT_MAX;

		// 최적화: TraceResults 전체가 아니라 '중복 제거된 액터(UniqueActors)'만 순회합니다.
		for (AActor* Actor : UniqueActors) 
		{
			FVector2D ActorScreenPos;
			if (PC->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ActorScreenPos))
			{
				float Dist = FVector2D::Distance(ActorScreenPos, CenterScreenPos);
				if (Dist < MinDist)
				{
					MinDist = Dist;
					FinalMainActor = Actor;
				}
			}
		}
	}
    
	// 컴포넌트에 최종 저장
	CameraObscuraComp->MainPhotoActor = FinalMainActor;
	
    // 3단계: 포인트 활성화 처리
	for (int32 i = 0; i < FinderPoints.Num(); i++)
	{
		UImage* Point = FinderPoints[i];
		if (!Point) continue;

		bool bHit = TraceResults[i].Key;
		AActor* HitActor = TraceResults[i].Value;

		// 2개 이상일 때만 필터링
		if (UniqueActors.Num() >= 2 && HitActor != FinalMainActor)
		{
			bHit = false;
			HitActor = nullptr;
		}

		CameraObscuraComp->SetPointActive(i, bHit, HitActor);
		Point->SetColorAndOpacity(bHit ? ActiveColor : DefaultColor);
	}

    // // 4단계: HeadSocket 또는 액터 중앙에 UI 표시
    // AActor* TargetActor = CameraObscuraComp->MainPhotoActor;
    if (TargetIndicatorWidget)
    {
        if (FinalMainActor)
        {
        	// 타겟이 이전과 달라졌을 때만 (새로운 타겟이 들어왔을 때만) 상태를 초기화하고 다시 검사합니다.
        	if (FinalMainActor != LastCheckedActor)
        	{
        		bIsAlreadyCaptured = false;      // 1. 여기서 false로 초기화
        		LastCheckedActor = FinalMainActor; // 2. 현재 타겟을 이전 타겟으로 덮어씌움

        		FName ActorName = NAME_None;
        		UPicturableComponent* PicturableComp = FinalMainActor->FindComponentByClass<UPicturableComponent>();
        		if (PicturableComp)
        		{
        			ActorName = PicturableComp->GetDatas().PicturableName;
        		}
                
        		// 저장된 배열에서 같은 이름 찾기
        		if (CameraObscuraComp->Cached_PS)
        		{
        			for (const FOwningPictureData& Data : CameraObscuraComp->Cached_PS->OwningPictureArray)
        			{
        				if (Data.PicturableDatas.PicturableName == ActorName && !Data.IsDuplicate)
        				{
        					bIsAlreadyCaptured = true;
        					break;
        				}
        			}
        		}
        		TargetIndicatorWidget->SetColorAndOpacity(bIsAlreadyCaptured ? FLinearColor::Green : FLinearColor::Red);

        	}
        	
            FVector TargetWorldPos;
            USkeletalMeshComponent* SkelMesh =
                FinalMainActor->FindComponentByClass<USkeletalMeshComponent>();

            if (SkelMesh && SkelMesh->DoesSocketExist(FName("HeadSocket")))
            {
                TargetWorldPos = SkelMesh->GetSocketLocation(FName("HeadSocket"));
            }
            else
            {
                TargetWorldPos = FinalMainActor->GetActorLocation();
            }
        	
        	// 가시성이 이미 Visible이 아닐 때만 초기화 및 재생
        	if (TargetIndicatorWidget && TargetIndicatorWidget->GetVisibility() != ESlateVisibility::Visible)
        	{
        		TargetIndicatorWidget->SetVisibility(ESlateVisibility::Visible);
        		
        		if (TargetIndicatorAnim && !IsAnimationPlaying(TargetIndicatorAnim))
        		{
        			PlayAnimation(TargetIndicatorAnim,0.0f,0);
        		}
        	}
        	
        	// 애니메이션이 적용한 현재 Scale 가져오기
        	FVector2D AnimScale = TargetIndicatorWidget->GetRenderTransform().Scale;
        
        	// FOV Scale 보간
        	float NewScaleValue = FMath::FInterpTo(CurrentFOVScale, TargetFOVScale, GetWorld()->GetDeltaSeconds(), 10.0f);
        	CurrentFOVScale = NewScaleValue;

        	// 애니메이션 Scale에 FOV Scale 곱하기
        	TargetIndicatorWidget->SetRenderScale(AnimScale * CurrentFOVScale);
        	
        	
        	// UI 위치
            FVector2D ScreenPos;
            if (PC->ProjectWorldLocationToScreen(TargetWorldPos, ScreenPos))
            {
            	float DPIScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
            	FVector2D AdjustedPos = ScreenPos / DPIScale;
            	
            	
                UCanvasPanelSlot* CanvasSlot =
                    Cast<UCanvasPanelSlot>(TargetIndicatorWidget->Slot);
            	if (CanvasSlot)
            	{
            		// Alignment (0.5, 0.5)이므로 위젯 중앙이 AdjustedPos에 정확히 위치
            		CanvasSlot->SetPosition(AdjustedPos);
            		CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f)); // 혹시 코드에서도 보장
            	}
            }
        }
        else
        {
            TargetIndicatorWidget->SetVisibility(ESlateVisibility::Hidden);
        	ResetCachedActor();
        }
    }
}

bool UObscuraUI::TraceFromScreenPoint(UImage* PointImage, AActor*& OutHitActor)
{
    FVector2D ScreenPos = GetPointScreenCenter(PointImage);

    FVector WorldOrigin, WorldDir;
    if (!PC->DeprojectScreenPositionToWorld(
        ScreenPos.X, ScreenPos.Y,
        WorldOrigin, WorldDir))
    {
        return false;
    }

    FVector TraceEnd = WorldOrigin + WorldDir * TraceDistance;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(PC->GetPawn());
    Params.bTraceComplex = true;

    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, WorldOrigin, TraceEnd, ECC_GameTraceChannel5, Params
    );

    if (bHit && Hit.GetActor()&&Hit.GetActor()->ActorHasTag(FName("Picturable")))
    {
        OutHitActor = Hit.GetActor();
        return true;
    }

    OutHitActor = nullptr;
    return false;
}

FVector2D UObscuraUI::GetPointScreenCenter(UImage* PointImage)
{
	FGeometry Geo = PointImage->GetCachedGeometry();

	FVector2D PixelPos;
	FVector2D ViewportPos;

	USlateBlueprintLibrary::LocalToViewport(
		GetWorld(),
		Geo,
		Geo.GetLocalSize() * 0.5f,
		PixelPos,
		ViewportPos
	);

	return PixelPos;
}

void UObscuraUI::ResetObscura()
{
	// 모든 포인트 기본 색상으로 초기화
	for (UImage* Point : FinderPoints)
	{
		if (Point) Point->SetColorAndOpacity(DefaultColor);
	}
	
	
}

void UObscuraUI::OnCameraFOVUpdated(float FOV)
{
	TargetFOVScale = 90.0f / FOV;
}


void UObscuraUI::OnPlayerMentalityBarUpdated(float curMental,float MaxMental,float amount)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(curMental/MaxMental);
		if (amount<0)
		{
			if (MentalityReduceAnim)
			{
				PlayAnimation(MentalityReduceAnim);	
			}
		}
	}
}

void UObscuraUI::OnStaminaBarUpdated(float percent)
{
	if (PB_StaminaBar)
	{
		PB_StaminaBar->SetPercent(percent);
	}
}

void UObscuraUI::OnShotCountUpdated(int shotCount)
{
	if (Text_ObscuraCount)
	{
		Text_ObscuraCount->SetText(FText::AsNumber(shotCount));
		
		ResetCachedActor();
		
	}
}


void UObscuraUI::ObscuraCooltimeUpdate()
{
	if (!CameraObscuraComp) return;
	
	if (PB_ObscuraCooltimeBar)
	{
		float percent = CameraObscuraComp->GetObscuraCooltimePercent();
		
		PB_ObscuraCooltimeBar->SetPercent(percent);
	}
}

void UObscuraUI::OnObscuraBarReset()
{ 
	if (PB_ObscuraCooltimeBar)
	{
		PB_ObscuraCooltimeBar->SetPercent(0.f);	
	}
}

void UObscuraUI::ResetCachedActor()
{
	// 촬영 후에 가지고 있던 액터 초기화 시켜주기
	LastCheckedActor = nullptr; // 다음 번에 같은 액터가 잡혀도 다시 검사하도록 nullptr로 초기화
	bIsAlreadyCaptured = false; // 상태도 덩달아 초기화
}




