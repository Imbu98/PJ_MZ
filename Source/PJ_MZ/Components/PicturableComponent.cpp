#include "PicturableComponent.h"
#include "Character/Enemy/EnemyBase.h"

UPicturableComponent::UPicturableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPicturableComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPicturableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UPicturableComponent::GetScore() const
{
	return  Picturable.PicturableMaxScore;
}

void UPicturableComponent::SetStaticMesh()
{
	AEnemyBase* enemy =  Cast<AEnemyBase>(GetOwner());
	if (!enemy)
	{
		UStaticMeshComponent* staticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (staticMesh&&Picturable.PicturableStaticMesh.IsValid())
		{
			staticMesh->SetStaticMesh(Picturable.PicturableStaticMesh.Get());
		}
	}
}


