#include "Notifies/ObscuraNotify.h"
#include "InputActionValue.h"
#include "Character/Player/HT_Player.h"
#include "Character/Player/HT_PlayerController.h"

void UObscuraNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference) 
{
	
	AHT_Player* player = Cast<AHT_Player>(MeshComp->GetOwner());
	if (player)
	{
		if (bEquipObscura)
		{
			player->CreateObscuraWidget();	
		}
		else
		{
			player->RemoveObscuraWidget();
		}
		
	}
}
