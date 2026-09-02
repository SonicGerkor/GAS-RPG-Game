// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainDamageGameplayAbility.h"
#include "MainFireBlast.generated.h"

class AMainFireBall;

UCLASS()
class AURA_API UMainFireBlast : public UMainDamageGameplayAbility
{
	GENERATED_BODY()

public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AMainFireBall*> SpawnFireBalls();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMainFireBall> FireBallClass;
};
