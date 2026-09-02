// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
	
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelectedDelegate;
	
	void InitializeLoadSlots();
	
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModel(int32 Index) const;
	
	void LoadData();
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void LoadGameButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();
	
private:
	
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_3;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
};
