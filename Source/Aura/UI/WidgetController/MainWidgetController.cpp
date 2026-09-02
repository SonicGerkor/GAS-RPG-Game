// Copyright Gerkor Games

#include "MainWidgetController.h"

#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/AbilitySystem/Data/AbilityInfo.h"
#include "Aura/Player/MainPlayerController.h"
#include "Aura/Player/MainPlayerState.h"

void UMainWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;	
}

void UMainWidgetController::BroadcastInitialValues()
{
	// Overriden in child classes
}

void UMainWidgetController::BindCallbacksToDependencies()
{
	// Overriden in child classes
}

void UMainWidgetController::BroadcastAbilityInfo()
{
	if (!GetMainASC()->bStartupAbilitiesGiven) return;
    	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FMainAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(MainAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = MainAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = MainAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		
		AbilityInfoDelegate.Broadcast(Info);
	});
	
	MainAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

AMainPlayerController* UMainWidgetController::GetMainPC()
{
	if (MainPlayerController == nullptr)
	{
		MainPlayerController = Cast<AMainPlayerController>(PlayerController);
	}
	return MainPlayerController;
}

AMainPlayerState* UMainWidgetController::GetMainPS()
{
	if (MainPlayerState == nullptr)
	{
		MainPlayerState = Cast<AMainPlayerState>(PlayerState);
	}
	return MainPlayerState;
}

UMainAbilitySystemComponent* UMainWidgetController::GetMainASC()
{
	if (MainAbilitySystemComponent == nullptr)
	{
		MainAbilitySystemComponent = Cast<UMainAbilitySystemComponent>(AbilitySystemComponent);
	}
	return MainAbilitySystemComponent;
}

UMainAttributeSet* UMainWidgetController::GetMainAS()
{
	if (MainAttributeSet == nullptr)
	{
		MainAttributeSet = Cast<UMainAttributeSet>(AttributeSet);
	}
	return MainAttributeSet;
}
