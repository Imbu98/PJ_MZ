#include "ObscuraCameraComponent.h"

#include "Character/Player/HT_Player.h"
#include "Kismet/GameplayStatics.h"


UObscuraCameraComponent::UObscuraCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UObscuraCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 추후 업그레이드를 하게 되면 값을 바꿔주기
	MaxObscuraCooltime = 3.0f;
}

void UObscuraCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsObscraCooltime)
	{
		currentObscuraCooltime+=DeltaTime;
		
		if (currentObscuraCooltime>MaxObscuraCooltime)
		{
			IsObscraCooltime= false;
			currentObscuraCooltime = 0.f;
			AHT_Player* player =  Cast<AHT_Player>(GetOwner());

			if (player)
			{
				player-> OnObscuraCooltimeFinished.Broadcast();
			}
		}
	}
}

void UObscuraCameraComponent::InitPoints(int32 PointCount)
{
	PointActiveList.Init(false, PointCount);
	PointHitActors.Init(nullptr, PointCount);
	ActivePointCount = 0;
}


void UObscuraCameraComponent::SetPointActive(int32 Index, bool bActive, AActor* HitActor)
{
	if (!PointActiveList.IsValidIndex(Index)) return;

	bool bWas = PointActiveList[Index];
	PointActiveList[Index] = bActive;
	PointHitActors[Index] = HitActor;

	// 카운트 갱신
	if (bActive && !bWas)       ActivePointCount++;
	else if (!bActive && bWas)  ActivePointCount--;
}

float UObscuraCameraComponent::GetDamageMultiplier() const
{
	if (ActivePointCount >= 7) return 4.0f;
	if (ActivePointCount >= 5) return 3.0f;
	if (ActivePointCount >= 3) return 2.0f;
	if (ActivePointCount >= 1) return 1.5f;
	return 1.0f;
}

void UObscuraCameraComponent::ApplyShutterDamage()
{
	// 촬영횟수가 없으면 사망
	if (CurrentCanShotCount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::Printf(TEXT("OverFlow...Died")));
		
		return;
	}
	
	CurrentCanShotCount--;
	
	float FinalDamage = GetDamageMultiplier();
	
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::Printf(TEXT("Score:%f"),FinalDamage));
	
	
	
	
	
	
	// AActor* Target = GetPrimaryTarget();
	// if (!Target) return;
	//
	
	//
	// UGameplayStatics::ApplyDamage(
	// 	Target,
	// 	FinalDamage,
	// 	GetOwner()->GetInstigatorController(),
	// 	GetOwner(),
	// 	DamageTypeClass
	// );
	//
	// UE_LOG(LogTemp, Log,
	// 	TEXT("Shutter! Target: %s | Points: %d | Multiplier: %.1fx | Damage: %.1f"),
	// 	*Target->GetName(), ActivePointCount,
	// 	GetDamageMultiplier(), FinalDamage);
}

AActor* UObscuraCameraComponent::GetPrimaryTarget() const
{
	// TMap<AActor*, int32> HitCount;
	//
	// for (int32 i = 0; i < PointHitActors.Num(); i++)
	// {
	// 	if (!PointActiveList[i]) continue;
	// 	AActor* Actor = PointHitActors[i].Get();
	// 	if (!Actor) continue;
	// 	HitCount.FindOrAdd(Actor)++;
	// }
	//
	// AActor* Best = nullptr;
	// int32 MaxCount = 0;
	// for (auto& Pair : HitCount)
	// {
	// 	if (Pair.Value > MaxCount)
	// 	{
	// 		MaxCount = Pair.Value;
	// 		Best = Pair.Key;
	// 	}
	// }
	// return Best;
	return nullptr;
}

void UObscuraCameraComponent::InitShotCount()
{
	CurrentCanShotCount = MaxCanShotCount;
}





