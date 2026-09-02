// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

class UMainAbilitySystemComponent;

UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	
	UPassiveNiagaraComponent();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
	void ActivateIfEquipped(UMainAbilitySystemComponent* MainASC);
};
