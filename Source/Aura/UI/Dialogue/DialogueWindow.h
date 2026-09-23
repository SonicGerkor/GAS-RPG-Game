// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWindow.generated.h"

UCLASS()
class AURA_API UDialogueWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void DisplayLine(const FText& Speaker, const FText& Line);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void RequestAdvance();	
};
