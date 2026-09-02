// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

class ULootTiers;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

UCLASS()
class AURA_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	void SaveGameProgress(ULoadScreenSaveGame* SaveObject);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SaveSlotName, const int32 SlotIndex) const;
	static void DeleteSlot(const FString& SaveSlotName, const int32 SlotIndex);
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;
	
	void TravelToMap(UMVVM_LoadSlot* Slot);
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<ULootTiers> LootTiers;
	
	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	FString StartingMapName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> StartingMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	TMap<FString, TSoftObjectPtr<UWorld>> GameMaps;
	
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	
	void PlayerDied(const ACharacter* DeadCharacter);
	
protected:
	
	virtual void BeginPlay() override;
};
