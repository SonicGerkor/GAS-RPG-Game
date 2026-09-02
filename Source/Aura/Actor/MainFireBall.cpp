// Copyright Gerkor Games

#include "MainFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"
#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AMainFireBall::BeginPlay()
{
	Super::BeginPlay();
	
	StartOutgoingTimeline();
}

void AMainFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UMainAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void AMainFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FMainGameplayTags::Get().GameplayCue_FireBlast, CueParams);	
	}
	
	if (LoopSoundComponent)
	{
		LoopSoundComponent->Stop();
		LoopSoundComponent->DestroyComponent();
	}
	
	bHit = true;
}
