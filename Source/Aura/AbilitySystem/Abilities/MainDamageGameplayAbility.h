// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainGameplayAbility.h"
#include "Aura/MainAbilityTypes.h"
#include "Aura/Interaction/CombatInterface.h"
#include "MainDamageGameplayAbility.generated.h"

UCLASS()
class AURA_API UMainDamageGameplayAbility : public UMainGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector OverrideKnockbackDirection = FVector::ZeroVector,
		bool bOverrideDeathImpulseDirection = false,
		FVector OverrideDeathImpulseDirection = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 6.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const;
};
