// Copyright Gerkor Games

#include "CharacterSelectionHUD.h"
#include "Aura/UI/Widget/MainUserWidget.h"
#include "Aura/UI/WidgetController/CharSelectionWidgetController.h"
#include "Aura/UI/WidgetController/MainWidgetController.h"
#include "Blueprint/UserWidget.h"

void ACharacterSelectionHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(CharacterSelectionWidgetClass, TEXT("Character Selection Widget Class uninitialized, check BP_MainHud"));
	checkf(CharacterSelectionWidgetControllerClass, TEXT("Character Selection Widget Controller Class uninitialized, check BP_CharacterSelectionHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), CharacterSelectionWidgetClass);
	CharacterSelectionWidget = Cast<UMainUserWidget>(Widget);
	
	const FWidgetControllerParams Params(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	
	UCharSelectionWidgetController* WidgetController = GetCharacterSelectionWidgetController(Params);
	CharacterSelectionWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

UCharSelectionWidgetController* ACharacterSelectionHUD::GetCharacterSelectionWidgetController(const FWidgetControllerParams& Params)
{
	if (CharacterSelectionWidgetController == nullptr)
	{
		CharacterSelectionWidgetController = NewObject<UCharSelectionWidgetController>(this, CharacterSelectionWidgetControllerClass);
		CharacterSelectionWidgetController->SetWidgetControllerParams(Params);
		CharacterSelectionWidgetController->BindCallbacksToDependencies();
	}
	
	return CharacterSelectionWidgetController;
}
