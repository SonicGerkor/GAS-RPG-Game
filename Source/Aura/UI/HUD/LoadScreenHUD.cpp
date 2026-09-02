// Copyright Gerkor Games

#include "LoadScreenHUD.h"

#include "Aura/UI/ViewModel/MVVM_LoadScreen.h"
#include "Aura/UI/Widget/LoadScreenWidget.h"
#include "Blueprint/UserWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();
	
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();
	
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();
	
	LoadScreenViewModel->LoadData();
}
