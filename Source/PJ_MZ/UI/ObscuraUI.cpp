// Fill out your copyright notice in the Description page of Project Settings.


#include "ObscuraUI.h"

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

void UObscuraUI::UpdateAllPoints()
{
	// 1단계: 0번(가운데) 포인트로 Main 결정
	AActor* MainHitActor = nullptr;
	if (FinderPoints.Num() > 0 && FinderPoints[0])
	{
		TraceFromScreenPoint(FinderPoints[0], MainHitActor);
	}
	CameraObscuraComp->MainPhotoActor = MainHitActor;

	// 2단계: 전체 포인트 처리
	for (int32 i = 0; i < FinderPoints.Num(); i++)
	{
		UImage* Point = FinderPoints[i];
		if (!Point) continue;

		AActor* HitActor = nullptr;
		bool bHit = TraceFromScreenPoint(Point, HitActor);

		// Main 액터가 아니면 무시
		if (HitActor != MainHitActor)
		{
			bHit = false;
			HitActor = nullptr;
		}

		CameraObscuraComp->SetPointActive(i, bHit, HitActor);
		Point->SetColorAndOpacity(bHit ? ActiveColor : DefaultColor);
	}

	// 3단계: HeadSocket 위치에 UI 표시
	if (TargetIndicatorWidget)
	{
		if (MainHitActor)
		{
			FVector TargetWorldPos;

			USkeletalMeshComponent* SkelMesh =
				MainHitActor->FindComponentByClass<USkeletalMeshComponent>();

			if (SkelMesh && SkelMesh->DoesSocketExist(FName("HeadSocket")))
			{
				// HeadSocket 위치 사용
				TargetWorldPos = SkelMesh->GetSocketLocation(FName("HeadSocket"));
			}
			else
			{
				// SkelMesh 없으면 액터 중앙 사용
				TargetWorldPos = MainHitActor->GetActorLocation();
			}

			FVector2D ScreenPos;
			if (PC->ProjectWorldLocationToScreen(TargetWorldPos, ScreenPos))
			{
				TargetIndicatorWidget->SetVisibility(ESlateVisibility::Visible);

				UCanvasPanelSlot* CanvasSlot =
					Cast<UCanvasPanelSlot>(TargetIndicatorWidget->Slot);
				if (CanvasSlot)
				{
					CanvasSlot->SetPosition(ScreenPos);
				}
				
				if (TargetIndicatorAnim)
				{
					TargetIndicatorWidget->PlayAnimation(TargetIndicatorAnim);	
				}
				
			}
		}
		else
		{
			TargetIndicatorWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

bool UObscuraUI::TraceFromScreenPoint(UImage* PointImage, AActor*& OutHitActor)
{
	// 포인트 화면 중심 좌표 계산
	FVector2D ScreenPos = GetPointScreenCenter(PointImage);

	// 화면 좌표 → 월드 레이 변환
	FVector WorldOrigin, WorldDir;
	if (!PC->DeprojectScreenPositionToWorld(
		ScreenPos.X, ScreenPos.Y,
		WorldOrigin, WorldDir))
	{
		return false;
	}

	// 레이캐스트
	FHitResult Hit;
	FVector TraceEnd = WorldOrigin + WorldDir * TraceDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PC->GetPawn()); // 플레이어 본인 무시
	Params.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, WorldOrigin, TraceEnd, ECC_GameTraceChannel5, Params
	);

	if (bHit && Hit.GetActor())
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
		
		}
	
	}
	
}


void UObscuraUI::OnPlayerMentalityBarUpdated(float percent)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(percent);
	}
}

void UObscuraUI::OnStaminaBarUpdated(float percent)
{
	if (PB_MentalityBar)
	{
		PB_MentalityBar->SetPercent(percent);
	}
}

void UObscuraUI::OnShotCountUpdated(int shotCount)
{
	if (Text_ObscuraCount)
	{
		Text_ObscuraCount->SetText(FText::AsNumber(shotCount));
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




