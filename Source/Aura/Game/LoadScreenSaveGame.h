// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Aura/AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

enum class ECharacterGenderPreset : uint8;
struct FGameplayTag;
class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	FGameplayTag AbilityStatus = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	FGameplayTag AbilitySlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class Defaults")
	int32 AbilityLevel = 1;
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName ActorName = FName();
	
	UPROPERTY()
	FTransform Transform = FTransform();
	
	// Serialized variables from the Actor - Only those marked with SaveGame specifier
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT(BlueprintType)
struct FSavedMap
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString MapAssetName = FString();
	
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	/* Default Properties */
	
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FString PlayerName = FString("DefaultPlayerName");
	
	UPROPERTY()
	ECharacterGenderPreset SelectedCharacterPreset = ECharacterGenderPreset::Male; 
	
	UPROPERTY()
	FString MapName = FString("DefaultMapName");
	
	UPROPERTY()
	FString MapAssetName = FString("DefaultMapAssetName");
	
	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
	
	UPROPERTY()
	bool bFirstTime = true;
	
	/* Player Properties */
	
	UPROPERTY()
	int32 PlayerLevel = 1;
	
	UPROPERTY()
	int32 Experience = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;
	
	UPROPERTY()
	int32 SpellPoints = 0;
	
	/* Attributes */
	
	UPROPERTY()
	float Strength = 0;
	
	UPROPERTY()
	float Intelligence = 0;
	
	UPROPERTY()
	float Resilience = 0;
	
	UPROPERTY()
	float Vigor = 0;
	
	/* Gameplay Abilities */
	
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
	
	/* Map Actors */
	
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
