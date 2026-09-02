// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainGameplayAbility.h"
#include "MainPassiveAbility.generated.h"

UCLASS()
class AURA_API UMainPassiveAbility : public UMainGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void ReceiveDeactivation(const FGameplayTag& AbilityTag);
};
