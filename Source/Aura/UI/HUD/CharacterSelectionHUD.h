// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterSelectionHUD.generated.h"

class UMainUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;
class UCharSelectionWidgetController;
struct FWidgetControllerParams;

UCLASS()
class AURA_API ACharacterSelectionHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);
	
	UCharSelectionWidgetController* GetCharacterSelectionWidgetController(const FWidgetControllerParams& Params);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UMainUserWidget> CharacterSelectionWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainUserWidget> CharacterSelectionWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCharSelectionWidgetController> CharacterSelectionWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharSelectionWidgetController> CharacterSelectionWidgetControllerClass;
};
