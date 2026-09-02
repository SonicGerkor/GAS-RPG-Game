// Copyright Gerkor Games

#include "MVVM_LoadScreen.h"
#include "MVVM_LoadSlot.h"
#include "Aura/Game/MainGameInstance.h"
#include "Aura/Game/MainGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("SaveSlot0");
	LoadSlot_0->LoadSlotIndex = 0;
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("SaveSlot1");
	LoadSlot_0->LoadSlotIndex = 1;
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("SaveSlot2");
	LoadSlot_0->LoadSlotIndex = 2;
	LoadSlot_3 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotIndex = 3;
	LoadSlot_3->LoadSlotName = FString("SaveSlot3");
	
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlots.Add(3, LoadSlot_3);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModel(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::LoadData()
{
	const AMainGameModeBase* GameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(GameMode)) return;
	for (const TTuple Pair : LoadSlots)
	{
		const ULoadScreenSaveGame* SaveObject = GameMode->GetSaveSlotData(Pair.Value->LoadSlotName, Pair.Key);
		const FString PlayerName = SaveObject->PlayerName;
		const FString MapName = SaveObject->MapName;
		const TEnumAsByte<ESaveSlotStatus> Status = SaveObject->SaveSlotStatus;
		Pair.Value->SlotStatus = Status;
		Pair.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		
		Pair.Value->SetPlayerName(PlayerName);
		Pair.Value->SetMapName(MapName);
		Pair.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		Pair.Value->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	if (AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		LoadSlots[Slot]->SetPlayerName(EnteredName);
		LoadSlots[Slot]->SetMapName(MainGameMode->StartingMapName);
		LoadSlots[Slot]->SetPlayerLevel(1);
		LoadSlots[Slot]->SlotStatus = Taken;
		LoadSlots[Slot]->PlayerStartTag = MainGameMode->DefaultPlayerStartTag;
		LoadSlots[Slot]->MapAssetName = MainGameMode->StartingMap.ToSoftObjectPath().GetAssetName();
		
		MainGameMode->SaveSlotData(LoadSlots[Slot], Slot);
		LoadSlots[Slot]->InitializeSlot();
		
		if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(MainGameMode->GetGameInstance()))
		{
			MainGameInstance->LoadSlotName = LoadSlots[Slot]->LoadSlotName;
			MainGameInstance->LoadSlotIndex = LoadSlots[Slot]->LoadSlotIndex;
			MainGameInstance->PlayerStartTag = MainGameMode->DefaultPlayerStartTag;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Please Switch to Single Player Mode"));
	}
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelectedDelegate.Broadcast();
	for (const TTuple Pair : LoadSlots)
	{
		Pair.Value->EnableSelectSlotButtonDelegate.Broadcast(Pair.Key != Slot);
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::LoadGameButtonPressed()
{
	if (AMainGameModeBase* MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(this)); !LoadSlots.IsEmpty())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(MainGameMode->GetGameInstance());
		MainGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
		MainGameInstance->LoadSlotName = SelectedSlot->LoadSlotName;
		MainGameInstance->LoadSlotIndex = SelectedSlot->LoadSlotIndex;
		
		if (IsValid(SelectedSlot))
		{
			MainGameMode->TravelToMap(SelectedSlot);	
		}
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (!IsValid(SelectedSlot)) return;
	
	AMainGameModeBase::DeleteSlot(SelectedSlot->LoadSlotName, SelectedSlot->LoadSlotIndex);
	SelectedSlot->SlotStatus = Vacant;
	SelectedSlot->InitializeSlot();
	SelectedSlot->EnableSelectSlotButtonDelegate.Broadcast(true);
}
