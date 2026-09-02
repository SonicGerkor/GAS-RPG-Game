// Copyright Gerkor Games

#include "MainFireBlast.h"

#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Aura/Actor/MainFireBall.h"

FString UMainFireBlast::GetDescription(const int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>FIRE BLAST</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><Mana>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Launches %d balls of fire, in all directions, each coming </>"
		"<Default>back and exploding upon return, causing </>"
		"<Damage>%d</><Default> radial fire damage with a chance to BURN.</>"), 
		Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage);
}

FString UMainFireBlast::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level);
}

TArray<AMainFireBall*> UMainFireBlast::SpawnFireBalls()
{
	TArray<AMainFireBall*> FireBalls;
	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UMainAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AMainFireBall* FireBall = GetWorld()->SpawnActorDeferred<AMainFireBall>(
			FireBallClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		
		FDamageEffectParams ExplosionDamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBall->ExplosionDamageParams = ExplosionDamageEffectParams;
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return FireBalls; 
}
