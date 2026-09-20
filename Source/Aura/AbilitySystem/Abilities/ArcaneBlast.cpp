// Copyright Gerkor Games

#include "ArcaneBlast.h"

FString UArcaneBlast::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ARCANE BLAST</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Spawns a chain of Arcane Shards that spawn towards the enemy, causing radial damage of </>"
		"<Damage>%d</><Default> at the hit point.</>"), 
		Level, ManaCost, Cooldown, ScaledDamage);
}

FString UArcaneBlast::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level);
}
