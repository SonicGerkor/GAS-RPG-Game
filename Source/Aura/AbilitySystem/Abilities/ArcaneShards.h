// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

UCLASS()
class AURA_API UArcaneShards : public UMainDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;
};
