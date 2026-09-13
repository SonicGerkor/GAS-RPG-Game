// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainDamageGameplayAbility.h"
#include "MeteorShower.generated.h"

UCLASS()
class AURA_API UMeteorShower : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumMeteors = 11;
};
