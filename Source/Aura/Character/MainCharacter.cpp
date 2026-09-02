// Copyright Gerkor Games

#include "MainCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemComponent.h"
#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Aura/Game/LoadScreenSaveGame.h"
#include "Aura/Game/MainGameModeBase.h"
#include "Aura/Player/MainPlayerController.h"
#include "Aura/Player/MainPlayerState.h"
#include "Aura/UI/HUD/MainHUD.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AMainCharacter::AMainCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	CharacterClass = ECharacterClass::Elementalist;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Init ability actor info for the Server
	InitAbilityActorInfo();
	LoadProgress();
	
	if (AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		MainGameMode->LoadWorldState(GetWorld());
	}
}

void AMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AMainCharacter::LoadProgress()
{
	AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(MainGameMode)) return;
	
	ULoadScreenSaveGame* SaveData = MainGameMode->RetrieveInGameSaveData();
	if (!IsValid(SaveData)) return;
	
	if (UCharacterClassInfo* ClassInfo = UMainAbilitySystemLibrary::GetCharacterClassInfo(this))
	{
		if (const FCharacterPresetInfo* PresetInfo = ClassInfo->CharacterPresetInfo.Find(SaveData->SelectedCharacterPreset))
		{
			if (USkeletalMesh* PresetMesh = PresetInfo->Mesh.LoadSynchronous())
			{
				GetMesh()->SetSkeletalMesh(PresetMesh);
			}
			if (PresetInfo->AnimClass)
			{
				GetMesh()->SetAnimInstanceClass(PresetInfo->AnimClass);
			}
		}
	}
	
	AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(GetPlayerState());
	if (!IsValid(MainPlayerState)) return;
	
	if (SaveData->bFirstTime)
	{
		InitializeDefaultAttributes();
		AddCharacterAbilities();
	}
	else
	{
		if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(AbilitySystemComponent))
		{
			MainASC->AddCharacterAbilitiesFromSavedData(SaveData);
		}
		
		MainPlayerState->SetLevel(SaveData->PlayerLevel);
		MainPlayerState->SetXP(SaveData->Experience);
		MainPlayerState->SetAttributePoints(SaveData->AttributePoints);
		MainPlayerState->SetSpellPoints(SaveData->SpellPoints);
		
		UMainAbilitySystemLibrary::InitializeDefaultAttributesFromSavedData(this, AbilitySystemComponent, SaveData);
	}
}

int32 AMainCharacter::GetPlayerLevel_Implementation()
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	
	return MainPlayerState->GetPlayerLevel();
}

void AMainCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);
	
	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		AMainGameModeBase* MainGM = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (MainGM)
		{
			MainGM->PlayerDied(this);
		}
	});
	
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DeathTimerDelegate, DeathTime, false);
	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

int32 AMainCharacter::GetAttributePoints_Implementation() const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->GetAttributePoints();
}

int32 AMainCharacter::GetSpellPoints_Implementation() const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->GetSpellPoints();
}

int32 AMainCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->LevelUpInfo->LevelInfo[Level].AttributePointAward;
}

int32 AMainCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	return MainPlayerState->LevelUpInfo->LevelInfo[Level].SpellPointAward;
}

void AMainCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	MainPlayerState->AddLevel(InPlayerLevel);
	
	if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		MainASC->UpdateAbilityStatus(MainPlayerState->GetPlayerLevel());	
	}
}

void AMainCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	MainPlayerState->AddToAttributePoints(InAttributePoints);
}

void AMainCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	MainPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AMainCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	
	return MainPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AMainCharacter::GetXP_Implementation() const
{
	const AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	
	return MainPlayerState->GetXP();
}

void AMainCharacter::AddToXP_Implementation(int32 InXP)
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	
	MainPlayerState->AddXP(InXP);
}

void AMainCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AMainCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetController()))
	{
		MainPlayerController->ShowMagicCircle(DecalMaterial);
		MainPlayerController->bShowMouseCursor = false;
	}
}

void AMainCharacter::HideMagicCircle_Implementation()
{
	if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetController()))
	{
		MainPlayerController->HideMagicCircle();
		MainPlayerController->bShowMouseCursor = true;
	}
}

void AMainCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(MainGameMode)) return;
	
	ULoadScreenSaveGame* SaveData = MainGameMode->RetrieveInGameSaveData();
	if (!IsValid(SaveData)) return;
	
	AMainPlayerState* MainPlayerState = Cast<AMainPlayerState>(GetPlayerState());
	if (!IsValid(MainPlayerState)) return;
	
	SaveData->PlayerLevel = MainPlayerState->GetPlayerLevel();
	SaveData->Experience = MainPlayerState->GetXP();
	SaveData->AttributePoints = MainPlayerState->GetAttributePoints();
	SaveData->SpellPoints = MainPlayerState->GetSpellPoints();
	
	SaveData->Strength = UMainAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Intelligence = UMainAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Resilience = UMainAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
	SaveData->Vigor = UMainAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

	SaveData->PlayerStartTag = CheckpointTag;
	SaveData->bFirstTime = false;
	
	if (!HasAuthority()) return;
	
	UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(AbilitySystemComponent);
	
	FForEachAbility SaveAbilityDelegate;
	SaveData->SavedAbilities.Empty();
	SaveAbilityDelegate.BindLambda([this, MainASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
	{
		const FGameplayTag AbilityTag = MainASC->GetAbilityTagFromSpec(AbilitySpec);
		UAbilityInfo* AbilityInfo = UMainAbilitySystemLibrary::GetAbilityInfo(this);
		FMainAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		
		FSavedAbility SavedAbility;
		SavedAbility.GameplayAbility = Info.Ability;
		SavedAbility.AbilityTag = AbilityTag;
		SavedAbility.AbilityType = Info.AbilityTypeTag;
		SavedAbility.AbilityLevel = AbilitySpec.Level;
		SavedAbility.AbilitySlot = MainASC->GetSlotFromAbilityTag(AbilityTag);
		SavedAbility.AbilityStatus = MainASC->GetStatusFromAbilityTag(AbilityTag);
		
		SaveData->SavedAbilities.AddUnique(SavedAbility);
	});
	
	MainASC->ForEachAbility(SaveAbilityDelegate);
	
	MainGameMode->SaveGameProgress(SaveData);
}

void AMainCharacter::OnRep_Stunned()
{
	if (UMainAbilitySystemComponent* MainASC = Cast<UMainAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		const FMainGameplayTags GameplayTags = FMainGameplayTags::Get();
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);

		if (bIsStunned)
		{
			MainASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			MainASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AMainCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void AMainCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

void AMainCharacter::InitAbilityActorInfo()
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState);
	MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
	Cast<UMainAbilitySystemComponent>(MainPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = MainPlayerState->GetAbilitySystemComponent();
	AttributeSet = MainPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FMainGameplayTags::Get().Debuff_Fire, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::BurnTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FMainGameplayTags::Get().Debuff_Lightning, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);
	
	if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(GetController()))
	{
		if (AMainHUD* MainHUD = Cast<AMainHUD>(MainPlayerController->GetHUD()))
		{
			MainHUD->InitOverlay(MainPlayerController, MainPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
