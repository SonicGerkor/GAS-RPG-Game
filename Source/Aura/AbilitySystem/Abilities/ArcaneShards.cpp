// Copyright Gerkor Games

#include "ArcaneShards.h"

FString UArcaneShards::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ARCANE SHARDS</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Summons shards of Arcane Energy, causing radial arcane damage of </>"
		"<Damage>%d</><Default> at the shard origin.</>"), 
		Level, ManaCost, Cooldown, ScaledDamage);
}

FString UArcaneShards::GetNextLevelDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ARCANE SHARDS</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Summons %d shards of Arcane Energy, causing radial arcane damage of </>"
		"<Damage>%d</><Default> at the shard origin.</>"), 
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards - 1), ScaledDamage);
}
