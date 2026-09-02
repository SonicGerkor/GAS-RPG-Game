// Copyright Gerkor Games

#include "ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "Aura/MainAbilityTypes.h"
#include "Aura/MainGameplayTags.h"
#include "Aura/AbilitySystem/MainAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/MainAttributeSet.h"
#include "Aura/AbilitySystem/Data/CharacterClassInfo.h"
#include "Aura/Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct MainDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	
	MainDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, CriticalHitChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, CriticalHitResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, CriticalHitDamage, Source, false)
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, LightningResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMainAttributeSet, PhysicalResistance, Target, false)
	}
};

static const MainDamageStatics& DamageStatics()
{
	static MainDamageStatics DStatic;
	return DStatic;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(MainDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(MainDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(MainDamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDef) const
{
	const FMainGameplayTags& GameplayTags = FMainGameplayTags::Get();
	
	// Determine Debuff
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& DebuffTypeTag = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);
		if (TypeDamage > -.5f) // .f Padding for floating point imprecision
		{
			// Determine if there was a successful debuff.
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Info_Chance, false, -1.f);
			
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageTypeTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDef[ResistanceTag], EvaluateParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) * 0.01f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				
				UMainAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UMainAbilitySystemLibrary::SetDamageType(ContextHandle, DamageTypeTag);
				
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Info_Damage, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Info_Frequency, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Info_Duration, false, -1.f);
				UMainAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UMainAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				UMainAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FMainGameplayTags& Tags = FMainGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
		
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);
	
	// Get Damage SetByCallerMagnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : Tags.DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s]"), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false, 0.f);
		
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
		DamageTypeValue *= (100.f - Resistance) * 0.01f;
		
		// Determine Radial Damage and Scale it.
		if (UMainAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			if (ICombatInterface* Combat = Cast<ICombatInterface>(TargetAvatar))
			{
				Combat->GetOnDamageSignature().AddLambda([&](float DamageTaken)
				{
					DamageTypeValue = DamageTaken;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar, 
				DamageTypeValue, 
				0.f,
				UMainAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UMainAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UMainAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}
		
		Damage += DamageTypeValue;
	}
	
	// Capture BlockChance on Target and determine if there was a successful Block.
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	// If Block, halve the damage.
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	UMainAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	Damage = !bBlocked ? Damage : Damage * 0.5f; 
	
	// Capture the Target Armor value
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	
	// Capture the Source Armor Penetration value
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UMainAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	// Default Values
	float ArmorPenetrationCoefficient = 0.25f;
	float EffectiveArmorCoefficient = 0.33f;
	float CriticalHitResistanceCoefficient = 0.15f;
	
	// CurveTable Coefficient Values
	if (CharacterClassInfo)
	{
		const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
		ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
		
		const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
		EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
		
		const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
		CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(SourcePlayerLevel);
	}
	
	// ArmorPenetration ignores a % of the Target's Armor.
	const float EffectiveArmor = (TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f);
	// Armor  ignores a % of incoming Damage.
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) * 0.01f;
	
	// CriticalHitDamage (Source) & CriticalHitResistance (Target)
	// Capture CriticalHitDamage on Target and determine if there was a successful Block.
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	
	// If CriticalHit, Double the Damage + Bonus.
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bIsCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	UMainAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);
	Damage = !bIsCriticalHit ? Damage : (Damage * 2.f) + SourceCriticalHitDamage;

	const FGameplayModifierEvaluatedData EvaluatedData(UMainAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
