// Copyright Gerkor Games

#include "LightningStorm.h"

FString ULightningStorm::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>LIGHTNING STORM</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Brings down a lightning bolt from the sky, causing radial arcane damage of </>"
		"<Damage>%d</><Default> at the lightning bolt origin.</>"), 
		Level, ManaCost, Cooldown, ScaledDamage);
}

FString ULightningStorm::GetNextLevelDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>LIGHTNING STORM</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Brings down %d lightning bolts from the sky, causing radial arcane damage of </>"
		"<Damage>%d</><Default> at the lightning bolts origin.</>"), 
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumLightnings - 1), ScaledDamage);
}
