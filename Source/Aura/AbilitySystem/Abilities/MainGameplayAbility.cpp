// Copyright Gerkor Games

#include "MainGameplayAbility.h"

#include "Aura/AbilitySystem/MainAttributeSet.h"

FString UMainGameplayAbility::GetDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.", Level);
}

FString UMainGameplayAbility::GetNextLevelDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage.</>"), Level);
}

FString UMainGameplayAbility::GetLockedDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until\nLevel: %d</>"), Level);
}

float UMainGameplayAbility::GetManaCost(const float InLevel) const
{
	float ManaCost = 0.f;
	const UGameplayEffect* CostGameplayEffect = GetCostGameplayEffect();
	if (CostGameplayEffect == nullptr) return ManaCost;
	
	for (FGameplayModifierInfo Mod : CostGameplayEffect->Modifiers)
	{
		if (Mod.Attribute == UMainAttributeSet::GetManaAttribute())
		{
			Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
			break;
		}
	}
	return ManaCost;
}

float UMainGameplayAbility::GetCooldown(const float InLevel) const
{
	float Cooldown = 0.f;
	const UGameplayEffect* CooldownGameplayEffect = GetCooldownGameplayEffect();
	if (CooldownGameplayEffect == nullptr) return Cooldown;
	
	CooldownGameplayEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	
	return Cooldown;
}
