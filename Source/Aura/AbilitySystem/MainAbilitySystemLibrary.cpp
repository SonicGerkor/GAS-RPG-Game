// Copyright Gerkor Games

#include "MainAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/MainAbilityTypes.h"
#include "Aura/MainGameplayTags.h"
#include "Aura/Game/LoadScreenSaveGame.h"
#include "Aura/Game/MainGameModeBase.h"
#include "Aura/Interaction/CombatInterface.h"
#include "Aura/Player/MainPlayerState.h"
#include "Aura/UI/HUD/MainHUD.h"
#include "Aura/UI/WidgetController/MainWidgetController.h"
#include "Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

bool UMainAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AMainHUD*& OutMainHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutMainHUD = Cast<AMainHUD>(PC->GetHUD());
		if (OutMainHUD)
		{
			AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutParams.AttributeSet = AS;
			OutParams.AbilitySystemComponent = ASC;
			OutParams.PlayerState = PS;
			OutParams.PlayerController = PC;
			
			return true;
		}
	}
	
	return false;
}

UOverlayWidgetController* UMainAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AMainHUD* MainHUD;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, MainHUD))
	{
		return MainHUD->GetOverlayWidgetController(WCParams);
	}
	
	return nullptr;
}

UAttributeMenuWidgetController* UMainAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AMainHUD* MainHUD;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, MainHUD))
	{
		return MainHUD->GetAttributeMenuWidgetController(WCParams);
	}
	
	return nullptr;
}

USpellMenuWidgetController* UMainAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AMainHUD* MainHUD;

	if (MakeWidgetControllerParams(WorldContextObject, WCParams, MainHUD))
	{
		return MainHUD->GetSpellMenuWidgetController(WCParams);
	}
	
	return nullptr;
}

void UMainAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr || ASC == nullptr) return;

	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UMainAbilitySystemLibrary::InitializeDefaultAttributesFromSavedData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SavedGame)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr || ASC == nullptr) return;
	
	const FMainGameplayTags& GameplayTags =FMainGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SavedGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SavedGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SavedGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SavedGame->Vigor);
	
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	
	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UMainAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr || ASC == nullptr) return;

	for (const TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UMainAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MainGameMode == nullptr) return nullptr;
	return MainGameMode->CharacterClassInfo;
}

UAbilityInfo* UMainAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MainGameMode == nullptr) return nullptr;
	return MainGameMode->AbilityInfo;
}

ULootTiers* UMainAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
	const AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MainGameMode == nullptr) return nullptr;
	return MainGameMode->LootTiers;
}

void UMainAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bIsRadialDamage)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsRadialDamage(bIsRadialDamage);
	}
}

void UMainAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float RadialDamageInnerRadius)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetRadialDamageInnerRadius(RadialDamageInnerRadius);
	}
}

void UMainAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float RadialDamageOuterRadius)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetRadialDamageOuterRadius(RadialDamageOuterRadius);
	}
}

void UMainAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& RadialDamageOrigin)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetRadialDamageOrigin(RadialDamageOrigin);
	}
}

void UMainAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bIsSuccessfulDebuff)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsSuccessfulDebuff(bIsSuccessfulDebuff);
	}
}

void UMainAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float DebuffDamage)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDebuffDamage(DebuffDamage);
	}
}

void UMainAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float DebuffFrequency)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDebuffFrequency(DebuffFrequency);
	}
}

void UMainAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float DebuffDuration)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDebuffDuration(DebuffDuration);
	}
}

void UMainAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		MainEffectContext->SetDamageType(DamageType);
	}
}

void UMainAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UMainAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetKnockbackForce(InForce);
	}
}

bool UMainAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->IsRadialDamage();
	}
	
	return false;
}

float UMainAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetRadialDamageInnerRadius();
	}
	
	return 0.f;
}

float UMainAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetRadialDamageOuterRadius();
	}
	
	return 0.f;
}

FVector UMainAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetRadialDamageOrigin();
	}
	
	return FVector::ZeroVector;
}

FVector UMainAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetKnockbackForce();
	}
	
	return FVector::ZeroVector;
}

FVector UMainAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetDeathImpulse();
	}
	
	return FVector::ZeroVector;
}

bool UMainAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->IsSuccessfulDebuff();
	}
	
	return false;
}

float UMainAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetDebuffDamage();
	}
	
	return 0.f;
}

float UMainAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetDebuffFrequency();
	}
	
	return 0.f;
}

float UMainAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->GetDebuffDuration();
	}
	
	return 0.f;
}

FGameplayTag UMainAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (MainGameplayEffectContext->GetDamageType().IsValid())
		{
			return *MainGameplayEffectContext->GetDamageType();
		}
	}
	
	return FGameplayTag();
}

bool UMainAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->IsBlockedHit();
	}
	
	return false;
}

void UMainAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsBlockedHit(bIsBlockedHit);
	}
}

bool UMainAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainGameplayEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainGameplayEffectContext->IsCriticalHit();
	}
	
	return false;
}

void UMainAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		MainEffectContext->SetIsCriticalHit(bIsCriticalHit);
	}
}

void UMainAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutActors, const TArray<AActor*>& IgnoreActors, float Radius, const FVector& SphereLocation)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(IgnoreActors);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

void UMainAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestActors, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestActors = Actors;
		return;
	}
	
	TArray<TPair<double, AActor*>> DistancePairs;
	DistancePairs.SetNumUninitialized(Actors.Num());
 
	for (int32 i = 0; i < Actors.Num(); ++i)
	{
		const double DistanceSq = FVector::DistSquared(Origin, Actors[i]->GetActorLocation());
		DistancePairs[i] = TPair<double, AActor*>(DistanceSq, Actors[i]);
	}
 
	const int32 NumToFind = FMath::Min(MaxTargets, DistancePairs.Num());
 
	std::nth_element(DistancePairs.GetData(), DistancePairs.GetData() + NumToFind, DistancePairs.GetData() + DistancePairs.Num(),
	[](const TPair<double, AActor*>& A, const TPair<double, AActor*>& B)
	{
	   return A.Key < B.Key;
	});
 
	OutClosestActors.Reserve(NumToFind);
	for (int32 i = 0; i < NumToFind; ++i)
	{
		OutClosestActors.Add(DistancePairs[i].Value);
	}
}

bool UMainAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	
	return !bFriends;
}

int32 UMainAbilitySystemLibrary::GetXPRewardForClass(const UObject* WorldContextObject, const ECharacterClass CharacterClass, const int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;
	
	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
	
	return static_cast<int32>(XPReward);
}

FGameplayEffectContextHandle UMainAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	const FMainGameplayTags& GameplayTags =FMainGameplayTags::Get();
	const AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, Params.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, Params.KnockbackForce);
	
	SetIsRadialDamage(EffectContextHandle, Params.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, Params.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, Params.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, Params.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(Params.DamageGameplayEffectClass, Params.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Info_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Info_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Info_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Info_Duration, Params.DebuffDuration);
	
	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	
	return EffectContextHandle;
}

TArray<FRotator> UMainAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, const float Spread, const int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread * 0.5f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	
	return Rotators;
}

TArray<FVector> UMainAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, const float Spread, const int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread * 0.5f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	
	return Vectors;
}

void UMainAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UMainAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	const float FinalMagnitude = Magnitude == 0.f ? DamageEffectParams.KnockbackForceMagnitude : Magnitude;
	DamageEffectParams.KnockbackForce = KnockbackDirection * FinalMagnitude;
}

void UMainAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude)
{
	DeathImpulseDirection.Normalize();
	const float FinalMagnitude = Magnitude == 0.f ? DamageEffectParams.DeathImpulseMagnitude : Magnitude;
	DamageEffectParams.DeathImpulse = DeathImpulseDirection * FinalMagnitude;
}

void UMainAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}
