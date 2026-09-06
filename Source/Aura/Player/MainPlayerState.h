// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChanged, int32 /* StatValue */, bool /* bLevelUp */);

UCLASS()
class AURA_API AMainPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	AMainPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	void AddLevel(int32 NewLevel);
	void SetLevel(int32 NewLevel);
	
	FORCEINLINE int32 GetXP() const { return XP; }
	void AddXP(int32 XPToAdd);
	void SetXP(int32 NewXP);
	
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	
	UFUNCTION(BlueprintCallable)
	void SetAttributePoints(int32 NewAttributePoints);
	
	UFUNCTION(BlueprintCallable)
	void AddToAttributePoints(int32 NewAttributePoints);
	
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	void SetSpellPoints(int32 NewSpellPoints);
	void AddToSpellPoints(int32 NewSpellPoints);
	
	// Delegates
	FOnPlayerLevelChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
