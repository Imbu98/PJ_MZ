#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObscuraUI.generated.h"

UCLASS()
class PJ_MZ_API UObscuraUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<TObjectPtr<class UImage>> FinderPoints;
	
	// 레이 최대 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	float TraceDistance = 5000.f;
	
	// 포인트 기본 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	FLinearColor DefaultColor = FLinearColor(1.f, 1.f, 1.f, 0.6f);

	// 포인트 활성화 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Obscura")
	FLinearColor ActiveColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
	
	// 카메라 컴포넌트 참조 (초기화 시 외부에서 주입)
	UPROPERTY(BlueprintReadWrite, Category="Camera Obscura")
	TObjectPtr<class UObscuraCameraComponent> CameraObscuraComp;

	// 감지 활성화 여부 (카메라 열렸을 때만 true)
	UPROPERTY(BlueprintReadWrite, Category="Camera Obscura")
	bool bDetectionActive = false;
	
private:
	// player 정신력수치 프로그레스 바
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> PB_MentalityBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> PB_StaminaBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> PB_ObscuraCooltimeBar;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_ObscuraCount;
		
	UFUNCTION()
	void OnPlayerMentalityBarUpdated(float percent);
	
	// 스프린트 게이지
	UFUNCTION()
	void OnStaminaBarUpdated(float percent);
	
	UFUNCTION()
	void OnShotCountUpdated(int shotCount);

	UFUNCTION()
	void ObscuraCooltimeUpdate();
	
	UFUNCTION()
	void OnObscuraBarReset();
	
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	APlayerController* PC;

	// 라인 트레이스 관련 함수들
	void UpdateAllPoints();
	bool TraceFromScreenPoint(UImage* PointImage, AActor*& OutHitActor);
	FVector2D GetPointScreenCenter(UImage* PointImage);
	void ResetObscura();
	
	void BindingEvent();
	
	
};
