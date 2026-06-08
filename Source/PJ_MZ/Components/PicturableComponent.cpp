#include "PicturableComponent.h"

#include "AIController.h"
#include "Character/Enemy/EnemyAIControllerBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "ETC/Picturable/PicturableBase.h"

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

void UPicturableComponent::PictureTaken()
{
	AEnemyBase* enemy =  Cast<AEnemyBase>(GetOwner());
	// 적이면 AIController에서 적에 대한 처리
	if (enemy)
	{
		AEnemyAIControllerBase* aiController =  Cast<AEnemyAIControllerBase>(enemy->GetController());
		
		if (aiController)
		{
			aiController->PictureTaken();
		}
	}
	// 아니면 오브젝트에 대한 처리
	else
	{
		APicturableBase* picturable =  Cast<APicturableBase>(GetOwner());
		if (picturable)
		{
			picturable->PictureTaken();
		}
	}
}


