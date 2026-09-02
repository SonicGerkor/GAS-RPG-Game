// Copyright Gerkor Games

#include "Electrocute.h"

FString UElectrocute::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ELECTROCUTE</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Emits a beam of lightning, connecting with the target, repeatedly causing </>"
		"<Damage>%d</><Default> electric damage with a chance to STUN.</>"), 
		Level, ManaCost, Cooldown, ScaledDamage);
}

FString UElectrocute::GetNextLevelDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ELECTROCUTE</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Emits a beam of lightning, connecting with the target, propagating to %d nearby targets, causing </>"
		"<Damage>%d</><Default> electric damage with a chance to STUN.</>"), 
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
}
