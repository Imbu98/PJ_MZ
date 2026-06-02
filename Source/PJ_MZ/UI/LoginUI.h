#pragma once

#include "CoreMinimal.h"
#include "MZ_Datas.h"
#include "Blueprint/UserWidget.h"
#include "LoginUI.generated.h"

enum class EUIState : uint8
{
	LOGIN,
	REGISTER,
};

UCLASS()
class PJ_MZ_API ULoginUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual  void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	// ===============================
	// 로그인
	// ===============================
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UOverlay> LoginOverlay; 	// 로그인 Overlay
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UEditableTextBox> Input_LoginID; 	// 로그인 ID

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> Input_LoginPW; 	// 로그인 PW
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_ShowLoginPW; 	// 로그인 PW 보이기 버튼
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_ShowRegisterOverlay; 	// 회원가입 overlay 띄우기 버튼
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_Login; 	// 로그인 버튼
	
	UFUNCTION()
	void OnClickShowLoginPassword();
	
	UFUNCTION()
	void OnClickShowRegisterOverlay();
	
	UFUNCTION()
	void OnClickLogin();
	
	// ===============================
	// 회원가입
	// ===============================
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UOverlay> RegisterOverlay; 	// 회원가입 Overlay
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> Input_RegisterID; 	// 회원가입 ID
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> Input_RegisterPW; 	// 회원가입 PW
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> Input_RegisterNick; 	// 회원가입 Nickname
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> Btn_ShowRegisterPW; 	// 회원가입 PW 보이기 버튼
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_Register; 	// 현재 정보들로 등록버튼
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_Back; 	// 회원가입 overlay 없애기 버튼
	
	
	UFUNCTION()
	void OnClickShowRegisterPassword(); // 회원가입 비밀번호 보이기
	
	UFUNCTION()
	void OnClickBack(); // 회원가입창 끄기
	
	UFUNCTION()
	void OnClickRegister(); // 회원가입
	
	// DB 델리게이트
	UFUNCTION()
	void OnLoginResult(bool bSuccess, const FString& UserId, const FString& Nickname,int32 StageFlags);

	UFUNCTION()
	void OnRegisterResult(bool bSuccess, const FString& ErrorMessage);
	
	// ===============================
	// 스테이지 선택
	// ===============================
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> StageSelectOverlay;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_Prev;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_Next;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_StageSelect;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_UserName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Btn_StartGame;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Img_Locked;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<class UDataTable> DT_StageSelectInfo;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<class UStageSelectUI> StageSelectUIFactory;
	
	UPROPERTY()
	TObjectPtr<UStageSelectUI> StageSelectUIWidget;
	
	UPROPERTY()
	TArray<FStageSelectData> StageSelectData;
	
	UPROPERTY()
	TArray<TObjectPtr<UStageSelectUI>> StageSelectUIArray;

	
	UFUNCTION()
	void SetStageSelectOverlay(const int32 stageFlags);
	
	int32 CurrentIndex = 0;
	
	int32 StageFlags=0;
	
	

	UFUNCTION()
	void OnClickNext();

	UFUNCTION()
	void OnClickPrev();
	
	UFUNCTION()
	void OnClickStartGame();

	void ScrollToIndex(int32 Index);
	void UpdateVisibility();
	
	// 목표 오프셋
	float TargetScrollOffset = 0.f;
	bool bIsScrolling = false;

	// 보간 속도
	UPROPERTY(EditDefaultsOnly, Category = "Scroll")
	float ScrollInterpSpeed = 8.f;
};
