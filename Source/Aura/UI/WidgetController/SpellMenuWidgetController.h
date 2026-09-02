// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MainWidgetController.h"
#include "Aura/MainGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

struct FGameplayTag;
class UMainUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelectedSignature, UMainUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UMainWidgetController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChangedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SelectAbility(UMainUserWidget* AbilityButton);
    
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spells Menu")
	FAbilitySelectedSignature AbilitySelectedDelegate;
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
	
private:
	
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 InSpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = { FMainGameplayTags::Get().Abilities_None, FMainGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	
	bool bWaitingForEquipSelection = false;
	
	FGameplayTag SelectedSlot;
};
