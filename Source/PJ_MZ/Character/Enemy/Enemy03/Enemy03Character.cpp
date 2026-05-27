

#include "Enemy03Character.h"


// Sets default values
AEnemy03Character::AEnemy03Character()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemy03Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy03Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

