// Copyright Gerkor Games

#include "MainDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UMainDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	const float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, DamageMagnitude);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UMainDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(
	AActor* TargetActor,
	FVector InRadialDamageOrigin, 
	bool bOverrideKnockbackDirection, 
	FVector OverrideKnockbackDirection,
	bool bOverrideDeathImpulseDirection,
	FVector OverrideDeathImpulseDirection, 
	bool bOverridePitch,
	float PitchOverride) const
{
	FDamageEffectParams DamageEffectParams;
	
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParams.KnockbackForceMagnitude = KnockbackForceMagnitude;
	DamageEffectParams.KnockbackChance = KnockbackChance;
	
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		
		if (!bOverrideKnockbackDirection)
		{
			DamageEffectParams.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
		if (!bOverrideDeathImpulseDirection)
		{
			DamageEffectParams.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		}
	}
	
	if (bOverrideKnockbackDirection)
	{
		OverrideKnockbackDirection.Normalize();
		DamageEffectParams.KnockbackForce = OverrideKnockbackDirection * KnockbackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = OverrideKnockbackDirection.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DamageEffectParams.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}
	
	if (bOverrideDeathImpulseDirection)
	{
		OverrideDeathImpulseDirection.Normalize();
		DamageEffectParams.DeathImpulse = OverrideDeathImpulseDirection * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = OverrideDeathImpulseDirection.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			DamageEffectParams.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}
	
	if (bIsRadialDamage)
	{
		DamageEffectParams.bIsRadialDamage = bIsRadialDamage;
		DamageEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
		DamageEffectParams.RadialDamageOrigin = InRadialDamageOrigin;
	}
	
	return DamageEffectParams;
}

float UMainDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UMainDamageGameplayAbility::GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (!TaggedMontages.IsEmpty())
	{
		const int32 RandomNumber = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandomNumber];
	}
	return FTaggedMontage();
}
