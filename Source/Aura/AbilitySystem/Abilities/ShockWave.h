// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainDamageGameplayAbility.h"
#include "ShockWave.generated.h"

class AMainShockWave;

UCLASS()
class AURA_API UShockWave : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AMainShockWave*> SpawnShockWave();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	int32 NumShocks = 8;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShockWave")
	float SpawnSpread = 180.f;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMainShockWave> ShockWaveClass;
};
