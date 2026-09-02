// Copyright Gerkor Games

#include "AttributeMenuWidgetController.h"

#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/Player/MainPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	
	for (auto& Pair : GetMainAS()->TagsToAttributes)
	{
		GetMainASC()->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		});	
	}
	
	GetMainPS()->OnAttributePointsChangedDelegate.AddLambda([this](const int32 Points)
	{
		AttributePointsChangedDelegate.Broadcast(Points);
	});
	
	GetMainPS()->OnSpellPointsChangedDelegate.AddLambda([this](const int32 Points)
	{
		SpellPointsChangedDelegate.Broadcast(Points);
	});
}

void UAttributeMenuWidgetController::UpgradeAttributes(const FGameplayTag& AttributeTag)
{
	GetMainASC()->UpgradeAttributes(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	
	for (TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : GetMainAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	AttributePointsChangedDelegate.Broadcast(GetMainPS()->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(GetMainPS()->GetSpellPoints());
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	FMainAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
