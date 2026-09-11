// Copyright Gerkor Games

#include "HaloOfProtection.h"

#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"

FString UHaloOfProtection::GetDescription(const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 DamageReduce = DamageReductionCurve.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>HALO OF PROTECTION</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Generates a protective Halo around the character, </>"
		"<Default> reducing all types of incoming Damage by <Damage>%d</>.</>"), 
		Level, ManaCost, Cooldown, DamageReduce);
}

FString UHaloOfProtection::GetNextLevelDescription(const int32 Level)
{
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 DamageReduce = DamageReductionCurve.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>HALO OF PROTECTION</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Generates a protective Halo around the character, </>"
		"<Default> reducing all types of incoming Damage by <Damage>%d</>.</>"), 
		Level, ManaCost, Cooldown, DamageReduce);
}

void UHaloOfProtection::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (UMainAbilitySystemComponent* AuraASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->AddLooseGameplayTag(FMainGameplayTags::Get().Abilities_Passive_Protection);
	}
}

void UHaloOfProtection::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (UMainAbilitySystemComponent* AuraASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->RemoveLooseGameplayTag(FMainGameplayTags::Get().Abilities_Passive_Protection);
	}
}
