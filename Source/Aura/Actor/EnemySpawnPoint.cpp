// Copyright Gerkor Games

#include "EnemySpawnPoint.h"

#include "Aura/Character/EnemyCharacter.h"

void AEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	
	AEnemyCharacter* Enemy = World->SpawnActorDeferred<AEnemyCharacter>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
