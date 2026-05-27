// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MZ_Datas.h"
#include "Components/ActorComponent.h"
#include "PicturableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PJ_MZ_API UPicturableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPicturableComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	FPicturableDatas GetDatas() const;
	
	UFUNCTION()
	void SetInfo(FPicturableDatas picturableDatas);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FPicturableDatas PicturableDatas;
	
	
};
