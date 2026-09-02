// Copyright Gerkor Games

#include "MainAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "MainGameplayTags.h"
#include "Engine/Engine.h"

UMainAssetManager& UMainAssetManager::Get()
{
	check(GEngine);
	UMainAssetManager* MainAssetManager = Cast<UMainAssetManager>(GEngine->AssetManager);
	
	return *MainAssetManager;
}

void UMainAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FMainGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
