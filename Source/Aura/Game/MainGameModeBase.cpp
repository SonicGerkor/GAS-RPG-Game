// Copyright Gerkor Games

#include "MainGameModeBase.h"

#include "EngineUtils.h"
#include "LoadScreenSaveGame.h"
#include "MainGameInstance.h"
#include "Aura/MainLogChannels.h"
#include "Aura/Interaction/SaveGameInterface.h"
#include "Aura/UI/ViewModel/MVVM_LoadSlot.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GameMaps.Add(StartingMapName, StartingMap);
}

AActor* AMainGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	
		if (!Actors.IsEmpty())
		{
			AActor* SelectedActor = Actors[0];
			for (AActor* Actor : Actors)
			{
				if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
				{
					if (PlayerStart->PlayerStartTag == MainGameInstance->PlayerStartTag)
					{
						SelectedActor = PlayerStart;
						break;
					}
				}
			}
			return SelectedActor;
		}	
	}
	
	return nullptr;
}

void AMainGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	
	USaveGame* SaveGameObj = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObj);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}

void AMainGameModeBase::SaveGameProgress(ULoadScreenSaveGame* SaveObject)
{
	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		const FString InGameLoadSlotName = MainGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = MainGameInstance->LoadSlotIndex;
		MainGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
		
		UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
	}
}

ULoadScreenSaveGame* AMainGameModeBase::GetSaveSlotData(const FString& SaveSlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

void AMainGameModeBase::DeleteSlot(const FString& SaveSlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AMainGameModeBase::RetrieveInGameSaveData()
{
	if (const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		const FString InGameLoadSlotName = MainGameInstance->LoadSlotName;
		const int32 InGameLoadSlotIndex = MainGameInstance->LoadSlotIndex;
		
		return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
	}
	return nullptr;
}

void AMainGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetGameInstance());
	check(MainGameInstance);
	
	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(MainGameInstance->LoadSlotName, MainGameInstance->LoadSlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!IsValid(Actor) || !Actor->Implements<USaveGameInterface>()) continue;
			
			FSavedActor SaveActor;
			SaveActor.ActorName = Actor->GetFName();
			SaveActor.Transform = Actor->GetTransform();
			
			FMemoryWriter MemoryWriter(SaveActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(SaveActor);
		}
		
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGame, MainGameInstance->LoadSlotName, MainGameInstance->LoadSlotIndex);
	}
}

void AMainGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	const UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetGameInstance());
	check(MainGameInstance);
	
	if (UGameplayStatics::DoesSaveGameExist(MainGameInstance->LoadSlotName, MainGameInstance->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SavedGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(MainGameInstance->LoadSlotName, MainGameInstance->LoadSlotIndex));
		if (SavedGame == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load Saved Game slot."));
			return;
		}
		
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!IsValid(Actor) || !Actor->Implements<USaveGameInterface>()) continue;
			
			for (FSavedActor SavedActor : SavedGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveGameInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}
					
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					// Converts binary bytes back into variables.
					Actor->Serialize(Archive);
					
					ISaveGameInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

void AMainGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->LoadSlotName;
	const int32 SlotIndex = Slot->LoadSlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, GameMaps.FindChecked(Slot->GetMapName()));
}

FString AMainGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : GameMaps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AMainGameModeBase::PlayerDied(const ACharacter* DeadCharacter)
{
	const ULoadScreenSaveGame* SavedGame = RetrieveInGameSaveData();
	if (!IsValid(SavedGame)) return;
	
	UGameplayStatics::OpenLevel(DeadCharacter, FName(SavedGame->MapAssetName));
}

void AMainGameModeBase::SetSelectedCharacterPreset(ECharacterGenderPreset Preset)
{
	if (ULoadScreenSaveGame* SaveData = RetrieveInGameSaveData())
	{
		SaveData->SelectedCharacterPreset = Preset;
		SaveGameProgress(SaveData);
	}
}
