#pragma once

#include "CoreMinimal.h"
#include "Base/InteractPawnBase.h"
#include "UI/PopupUI.h"
#include "StageNPC.generated.h"

UCLASS()
class PJ_MZ_API AStageNPC : public AInteractPawnBase
{
	GENERATED_BODY()

public:
	AStageNPC();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void Interacted_Implementation() override;

	FOnPopupAction ConfirmDelegate;
	FOnPopupAction CancelDelegate;

	// 플레이어에게 카메라 줌
	UFUNCTION()
	void GivePlayerToCamera();

	void HandleFirstTalk(class AHT_PlayerState* PS);
	void HandleReturnTalk(AHT_PlayerState* PS);
	void ShowChoiceWidget();
	void OnConfirmExit();
	void OnCancelExit();

	UPROPERTY(EditAnywhere, Category="Dialogue")
	FName FirstTalkRowName = "StageNPC_First";

	UPROPERTY(EditAnywhere, Category="Dialogue")
	FName ReturnTalkRowName = "StageNPC_Return";

	// DialogueDataTable은 GameInstance나 PC에 두고
	// NPC는 RowName만 가지는게 더 깔끔
	UPROPERTY(EditAnywhere, Category="Dialogue")
	TObjectPtr<UDataTable> DialogueDataTable;

};
