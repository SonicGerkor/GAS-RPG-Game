// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainDamageGameplayAbility.h"
#include "MainProjectileSpell.generated.h"

class AMainProjectile;

UCLASS()
class AURA_API UMainProjectileSpell : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch = false, const float PitchOverride = 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMainProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 3;
};
