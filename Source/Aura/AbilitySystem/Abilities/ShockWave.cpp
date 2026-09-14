// Copyright Gerkor Games

#include "ShockWave.h"

#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Aura/Actor/MainShockWave.h"

FString UShockWave::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>SHOCK WAVE</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Launches %d bursts of air, in a fan pattern, causing </>"
		"<Default>back and exploding upon return, causing </>"
		"<Damage>%d</><Default> radial air damage, with a chance to STUN.</>"), 
		Level, ManaCost, Cooldown, NumShocks, ScaledDamage);
}

FString UShockWave::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level);
}

TArray<AMainShockWave*> UShockWave::SpawnShockWave()
{
	TArray<AMainShockWave*> ShockWaves;
	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UMainAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 180.f, NumShocks);
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AMainShockWave* ShockWave = GetWorld()->SpawnActorDeferred<AMainShockWave>(
			FireBallClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		ShockWave->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		const FDamageEffectParams ExplosionDamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		ShockWave->SetOwner(GetAvatarActorFromActorInfo());
		ShockWave->ExplosionDamageParams = ExplosionDamageEffectParams;
		ShockWaves.Add(ShockWave);
		ShockWave->FinishSpawning(SpawnTransform);
	}
	
	return ShockWaves; 
}
