// Copyright Gerkor Games

#include "PassiveNiagaraComponent.h"

#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		MainASC->ActivatePassiveEffectDelegate.AddUObject(this, &ThisClass::OnPassiveActivate);
		ActivateIfEquipped(MainASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				MainASC->ActivatePassiveEffectDelegate.AddUObject(this, &ThisClass::OnPassiveActivate);
				ActivateIfEquipped(MainASC);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UMainAbilitySystemComponent* MainASC)
{
	if (MainASC->bStartupAbilitiesGiven)
	{
		if (MainASC->GetStatusFromAbilityTag(PassiveSpellTag) == FMainGameplayTags::Get().Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
