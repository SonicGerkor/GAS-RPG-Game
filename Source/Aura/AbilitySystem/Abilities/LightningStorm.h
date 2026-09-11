// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainDamageGameplayAbility.h"
#include "LightningStorm.generated.h"

UCLASS()
class AURA_API ULightningStorm : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumLightnings = 11;
};
