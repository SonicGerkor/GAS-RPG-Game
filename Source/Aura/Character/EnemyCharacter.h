// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "Aura/Interaction/HighlightInterface.h"
#include "Aura/UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

class AMainAIController;
class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	
	AEnemyCharacter();

	virtual void BeginPlay() override;
	
	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	
	virtual int32 GetPlayerLevel_Implementation() override;
	
	virtual void Die(const FVector& DeathImpulse) override;
	
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	void SetLevel(const int32 InLevel) { Level = InLevel; }
	
protected:
	
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	
	void SetWidgetController();
	void BindDelegatesToBroadcast();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AMainAIController> MainAIController;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
