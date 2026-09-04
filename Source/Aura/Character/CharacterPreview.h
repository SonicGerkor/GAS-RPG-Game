// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CharacterPreview.generated.h"

UCLASS()
class AURA_API ACharacterPreview : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	
	ACharacterPreview();

	virtual void BeginPlay() override;
	
protected:
	
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
};
