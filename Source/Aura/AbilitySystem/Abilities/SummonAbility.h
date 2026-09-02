// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainGameplayAbility.h"
#include "SummonAbility.generated.h"


UCLASS()
class AURA_API USummonAbility : public UMainGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	
	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 MinionAmount = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 400.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
};
