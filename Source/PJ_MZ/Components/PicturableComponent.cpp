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

FPicturableDatas UPicturableComponent::GetDatas() const
{
	return  PicturableDatas;
}

void UPicturableComponent::SetInfo(FPicturableDatas picturableDataVal)
{
	PicturableDatas = picturableDataVal;
	
	AEnemyBase* enemy =  Cast<AEnemyBase>(GetOwner());
	if (!enemy)
	{
		UStaticMeshComponent* staticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (staticMesh&&PicturableDatas.PicturableStaticMesh.IsValid())
		{
			staticMesh->SetStaticMesh(PicturableDatas.PicturableStaticMesh.Get());
			staticMesh->SetCollisionProfileName(FName("Picturable"));
			staticMesh->SetVisibility(true);
			staticMesh->MarkRenderStateDirty();
		}
	}
}


