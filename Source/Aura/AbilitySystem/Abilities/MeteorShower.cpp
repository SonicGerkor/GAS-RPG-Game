// Copyright Gerkor Games

#include "MeteorShower.h"

FString UMeteorShower::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>METEOR SHOWER</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Calls a Meteor from the sky, causing radial fire damage of </>"
		"<Damage>%d</><Default> at the fire bolt center.</>"), 
		Level, ManaCost, Cooldown, ScaledDamage);
}

FString UMeteorShower::GetNextLevelDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>METEOR SHOWER</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Calls %d Meteors from the sky, causing radial fire damage of </>"
		"<Damage>%d</><Default> at the fire bolt center.</>"), 
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumMeteors - 1), ScaledDamage);
}
