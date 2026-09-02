// Copyright Gerkor Games

#include "MainPlayerState.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Net/UnrealNetwork.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMainAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UMainAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMainPlayerState, Level);
	DOREPLIFETIME(AMainPlayerState, XP);
	DOREPLIFETIME(AMainPlayerState, AttributePoints);
	DOREPLIFETIME(AMainPlayerState, SpellPoints);
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainPlayerState::AddLevel(const int32 NewLevel)
{
	Level += NewLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AMainPlayerState::SetLevel(const int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void AMainPlayerState::AddXP(const int32 XPToAdd)
{
	XP += XPToAdd;	
	OnXPChangedDelegate.Broadcast(XP);
}

void AMainPlayerState::SetXP(const int32 NewXP)
{
	XP = NewXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AMainPlayerState::SetAttributePoints(int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AMainPlayerState::AddToAttributePoints(const int32 NewAttributePoints)
{
	AttributePoints += NewAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AMainPlayerState::SetSpellPoints(int32 NewSpellPoints)
{
	SpellPoints = NewSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AMainPlayerState::AddToSpellPoints(const int32 NewSpellPoints)
{
	SpellPoints += NewSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AMainPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AMainPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AMainPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(OldAttributePoints);
}

void AMainPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(OldSpellPoints);
}
