// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PicturableBase.generated.h"

UCLASS()
class PJ_MZ_API APicturableBase : public AActor
{
	GENERATED_BODY()

public:
	APicturableBase();
	
	virtual void Tick(float DeltaTime) override;
	

protected:
	virtual void BeginPlay() override;


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPicturableComponent> PicturableComp;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UMaterialInstance> MaterialInstance; 
	
	UFUNCTION(BlueprintNativeEvent)
	void PictureTaken();
	

};
