// Copyright Gerkor Games

#pragma once

#include "CoreMinimal.h"
#include "MainWidgetController.h"
#include "OverlayWidgetController.h"
#include "CharSelectionWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AURA_API UCharSelectionWidgetController : public UMainWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
};
