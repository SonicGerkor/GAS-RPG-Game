// Copyright Gerkor Games

#include "CharSelectionWidgetController.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"

void UCharSelectionWidgetController::BroadcastInitialValues()
{
	// DOES NOTHING
}

void UCharSelectionWidgetController::BindCallbacksToDependencies()
{
	if (GetMainASC())
	{
		if (GetMainASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetMainASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
	}
}
