#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../MZ_Datas.h"
#include "PicturableBase.generated.h"

UCLASS()
class PJ_MZ_API APicturableBase : public AActor
{
	GENERATED_BODY()

public:
	APicturableBase();
	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USphereComponent> SphereComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;
	
public:
	UFUNCTION()
	float GetScore() const;
	

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FPicturableDatas Picturable;
	
};
