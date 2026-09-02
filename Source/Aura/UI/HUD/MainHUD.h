// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;
class UMainUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;

UCLASS()
class AURA_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	void InitOverlay(APlayerController* PlayerController, 
		APlayerState* PlayerState, 
		UAbilitySystemComponent* AbilitySystemComponent, 
		UAttributeSet* AttributeSet);
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& Params);
	
private:
	
	UPROPERTY()
	TObjectPtr<UMainUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
