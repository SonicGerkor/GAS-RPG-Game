// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Aura/Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	
	void InitializeSlot();
	
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndexDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButtonDelegate;
	
	UPROPERTY()
	FString LoadSlotName;
	
	UPROPERTY()
	int32 LoadSlotIndex;
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;
	
	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	FString MapAssetName;
	
	/* Field Notifies */
	
	void SetPlayerName(FString InPlayerName);
	FString GetPlayerName() const { return PlayerName; }
	
	void SetMapName(FString InMapName);
	FString GetMapName() const { return MapName; }
	
	void SetPlayerLevel(int32 InLevel);
	int32 GetPlayerLevel() const { return PlayerLevel; }
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 PlayerLevel;
};
