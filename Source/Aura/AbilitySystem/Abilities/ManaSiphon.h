// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainPassiveAbility.h"
#include "ManaSiphon.generated.h"

UCLASS()
class AURA_API UManaSiphon : public UMainPassiveAbility
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat ManaSiphonCurve;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
							 const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo,
							 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
							const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo,
							bool bReplicateEndAbility, bool bWasCancelled) override;
};
