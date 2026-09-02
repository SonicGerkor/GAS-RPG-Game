// Copyright Gerkor Games

#include "SpellMenuWidgetController.h"
#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/Data/AbilityInfo.h"
#include "Aura/Player/MainPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChangedDelegate.Broadcast(GetMainPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetMainASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			
			bool bEnableSpellPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
			FString Description;
			FString NextDescription;
			GetMainASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextDescription);
		}
		
		if (AbilityInfo)
		{
			FMainAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	
	GetMainASC()->AbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);
	
	GetMainPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChangedDelegate.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;
		
		bool bEnableSpellPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
		FString Description;
		FString NextDescription;
		GetMainASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextDescription);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	const FMainGameplayTags GameplayTags = FMainGameplayTags::Get();
	const int32 SpellPoints = GetMainPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetMainASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_None;
	}
	else
	{
		AbilityStatus = GetMainASC()->GetStatusFromSpec(*AbilitySpec);
	}
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	
	bool bEnableSpellPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpellPoints, bEnableEquip);		
	FString Description;
	FString NextDescription;
	GetMainASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetMainASC())
	{
		GetMainASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);	
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.Ability = FMainGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FMainGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
	
	const FGameplayTag SelectedStatus = GetMainASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FMainGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetMainASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;
	
	GetMainASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;
	
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
	
	StopWaitingForEquipSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 InSpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FMainGameplayTags GameplayTags= FMainGameplayTags::Get();
	
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpellPointsButton = InSpellPoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableSpellPointsButton = InSpellPoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpellPointsButton = InSpellPoints > 0;
	}
}

void USpellMenuWidgetController::SelectAbility(UMainUserWidget* AbilityButton)
{
	AbilitySelectedDelegate.Broadcast(AbilityButton);
}
