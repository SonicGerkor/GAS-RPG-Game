// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MainAbilitySystemGlobals.generated.h"

UCLASS()
class AURA_API UMainAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
