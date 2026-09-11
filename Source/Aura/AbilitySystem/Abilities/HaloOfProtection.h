// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainPassiveAbility.h"
#include "HaloOfProtection.generated.h"

UCLASS()
class AURA_API UHaloOfProtection : public UMainPassiveAbility
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat DamageReductionCurve;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility, bool bWasCancelled) override;
};
