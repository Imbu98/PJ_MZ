#include "AnimNotify_AttackHit.h"
#include "Character/Enemy/EnemyBase.h"

void UAnimNotify_AttackHit::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(
		MeshComp,
		Animation,
		EventReference);

	if (!MeshComp)
	{
		return;
	}

	if (AEnemyBase* Enemy =
		Cast<AEnemyBase>(MeshComp->GetOwner()))
	{
		Enemy->AttackHit();
	}
}