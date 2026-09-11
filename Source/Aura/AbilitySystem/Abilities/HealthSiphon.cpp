// Copyright Gerkor Games

#include "HealthSiphon.h"

#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"

FString UHealthSiphon::GetDescription(const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 DamageReduce = HealthSiphonCurve.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>HEALTH SIPHON</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Restores a small amount of HEALTH continuously.</>"), 
		Level, ManaCost, Cooldown);
}

FString UHealthSiphon::GetNextLevelDescription(const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 DamageReduce = HealthSiphonCurve.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>HEALTH OF PROTECTION</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Restores a small amount of HEALTH continuously.</>"), 
		Level, ManaCost, Cooldown);
}

void UHealthSiphon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (UMainAbilitySystemComponent* AuraASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FMainGameplayTags::Get().Abilities_Passive_HealthSiphon);
	}
}

void UHealthSiphon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (UMainAbilitySystemComponent* AuraASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FMainGameplayTags::Get().Abilities_Passive_HealthSiphon);
	}
}
