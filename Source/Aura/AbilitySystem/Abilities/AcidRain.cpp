// Copyright Gerkor Games

#include "AcidRain.h"

FString UAcidRain::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ACID RAIN</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Brings down acid rain from the sky, causing radial arcane damage of </>"
		"<Damage>%d</><Default> at the hit point.</>"), 
		Level, ManaCost, Cooldown, ScaledDamage);
}

FString UAcidRain::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level);
}
