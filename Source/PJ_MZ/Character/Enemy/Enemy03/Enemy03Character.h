// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "Enemy03Character.generated.h"

UCLASS()
class PJ_MZ_API AEnemy03Character : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy03Character();

protected:
	virtual void BeginPlay() override;

public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
