// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Aura/MainGameplayTags.h"
#include "ExecCalc_Damage.generated.h"

class UCharacterClassInfo;

UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	
	UExecCalc_Damage();
	
	void DetermineDebuff(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		const FGameplayEffectSpec& Spec, 
		FAggregatorEvaluateParameters EvaluateParameters, 
		const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDef) const;
	
	float ApplyDamageReductionByHaloOfProtection(
		float Damage, 
		const int32 TargetLevel,
		const UAbilitySystemComponent* TargetASC,
		const UCharacterClassInfo* TargetCharacterClassInfo) const;

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
