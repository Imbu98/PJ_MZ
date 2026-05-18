#include "ObscuraCameraComponent.h"

#include "Mz_GameInstance.h"
#include "Character/Player/HT_PlayerState.h"
#include "Character/Player/HT_Player.h"


UObscuraCameraComponent::UObscuraCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UObscuraCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AHT_Player* Player = Cast<AHT_Player>(GetOwner());
	if (!Player) return;

	AHT_PlayerState* PS = Player->GetPlayerState<AHT_PlayerState>();
	if (!PS) return;
	
	Cached_PS = PS;
}

void UObscuraCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (Cached_PS&&Cached_PS->IsObscraCooltime)
		
		Cached_PS->currentObscuraCooltime += DeltaTime;

	if (Cached_PS->currentObscuraCooltime >= Cached_PS->MaxObscuraCooltime)
	{
		Cached_PS->IsObscraCooltime = false;
		Cached_PS->currentObscuraCooltime = 0.f;
			
	}
		
	Cached_PS->OnObscuraCooltimeFinished.Broadcast();
	
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
	int32 CurrentCanShotCount =Cached_PS->GetCurrentCanShotCount();
	// 촬영횟수가 없으면 사망
	if (CurrentCanShotCount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::Printf(TEXT("OverFlow...Died")));
		
		return;
	}
	
	Cached_PS->SetCurrentCanShotCount(CurrentCanShotCount-1);
	
	
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

bool UObscuraCameraComponent::ObscuraCanShot()
{
	return Cached_PS->IsObscraCooltime;
}

float UObscuraCameraComponent::GetObscuraCooltimePercent()
{
	return Cached_PS->currentObscuraCooltime/Cached_PS->MaxObscuraCooltime;
}





