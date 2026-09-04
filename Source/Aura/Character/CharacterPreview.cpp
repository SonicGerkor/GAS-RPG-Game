// Copyright Gerkor Games

#include "CharacterPreview.h"

#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/Player/MainPlayerController.h"
#include "Aura/Player/MainPlayerState.h"
#include "Aura/UI/HUD/CharacterSelectionHUD.h"

ACharacterPreview::ACharacterPreview()
{	
	AbilitySystemComponent = CreateDefaultSubobject<UMainAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(false);
	
	AttributeSet = CreateDefaultSubobject<UMainAttributeSet>("AttributeSet");
	
	CharacterClass = ECharacterClass::Elementalist;
}

void ACharacterPreview::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
	InitializeDefaultAttributes();
}

void ACharacterPreview::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UMainAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}

void ACharacterPreview::InitializeDefaultAttributes() const
{
	Super::InitializeDefaultAttributes();
	
	UMainAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, 1, AbilitySystemComponent);
	
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}
