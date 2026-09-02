// Copyright Gerkor Games

#include "MainPassiveAbility.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"

void UMainPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		MainASC->DeactivatePassiveAbilityDelegate.AddUObject(this, &ThisClass::ReceiveDeactivation);
	}
}

void UMainPassiveAbility::ReceiveDeactivation(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
