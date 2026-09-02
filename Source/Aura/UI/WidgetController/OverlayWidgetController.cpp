// Copyright Gerkor Games

#include "OverlayWidgetController.h"

#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/Player/MainPlayerState.h"
#include "Engine/Engine.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetMainAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetMainAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetMainAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetMainAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetMainPS()->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetMainPS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel, bool bLevelUp)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetMainAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetMainAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetMainAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetMainAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});
	
	if (GetMainASC())
	{
		GetMainASC()->AbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);
		
		if (GetMainASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetMainASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
		
		GetMainASC()->EffectAssetTagsDelegate.AddLambda([this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);	
				}
			}	
		});	
	}
}

void UOverlayWidgetController::OnXPChanged(int32 XP)
{
	const ULevelUpInfo* LevelUpInfo = GetMainPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo in PlayerState."));
	
	const int32 Level = LevelUpInfo->FindLevelForXP(XP);
	const int32 MaxLevel = LevelUpInfo->LevelInfo.Num();
	
	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelRequirements = LevelUpInfo->LevelInfo[Level].LevelUpRequirement;
		const int32 PreviousLevelRequirement = LevelUpInfo->LevelInfo[Level - 1].LevelUpRequirement;
		
		const int32 DeltaLevelRequirement = LevelRequirements - PreviousLevelRequirement;
		const int32 XPForThisLevel = XP - PreviousLevelRequirement;
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FMainGameplayTags& GameplayTags = FMainGameplayTags::Get();
	
	FMainAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PrevSlot is a valid slot. Only if equipping an already equipped spell.
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FMainAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
