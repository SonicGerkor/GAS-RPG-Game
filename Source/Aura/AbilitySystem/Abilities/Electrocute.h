// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainBeamSpell.h"
#include "Electrocute.generated.h"


UCLASS()
class AURA_API UElectrocute : public UMainBeamSpell
{
	GENERATED_BODY()
	
public:
	
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
