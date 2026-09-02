// Copyright Gerkor Games

#include "MainAbilitySystemGlobals.h"
#include "Aura/MainAbilityTypes.h"

FGameplayEffectContext* UMainAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMainGameplayEffectContext();
}
