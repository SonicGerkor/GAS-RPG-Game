// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CharacterPreview.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreviewPawnReady);

UCLASS()
class AURA_API ACharacterPreview : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	
	ACharacterPreview();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Character Preview")
	UAbilitySystemComponent* GetPreviewAbilitySystemComponent() const { return GetAbilitySystemComponent(); }

	UFUNCTION(BlueprintCallable, Category = "Character Preview")
	UAttributeSet* GetPreviewAttributeSet() const { return GetAttributeSet(); }

	UPROPERTY(BlueprintAssignable, Category = "Character Preview")
	FOnPreviewPawnReady OnPreviewPawnReady;
	
protected:
	
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
};
