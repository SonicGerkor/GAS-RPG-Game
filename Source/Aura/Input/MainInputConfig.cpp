// Copyright Gerkor Games

#include "MainInputConfig.h"

const UInputAction* UMainInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMainInputAction& AbilityAction : AbilityInputActions)
	{
		if (AbilityAction.InputAction &&  AbilityAction.InputTag == InputTag)
		{
			return AbilityAction.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Ability Input Action for [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
