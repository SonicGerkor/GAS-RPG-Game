// Copyright Gerkor Games

#include "CharacterPreview.h"

#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/Player/MainPlayerState.h"

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
	
	OnPreviewPawnReady.Broadcast();
}

void ACharacterPreview::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UMainAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}

void ACharacterPreview::InitializeDefaultAttributes() const
{
	Super::InitializeDefaultAttributes();
	
	UMainAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, 1, AbilitySystemComponent);
}
